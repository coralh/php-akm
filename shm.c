#include "shm.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/errno.h>

#define SHM_OFFSET (sizeof(size_t))
#define SHM_HEAD_PTR(ptr) ((void *)(((unsigned char *)ptr) - SHM_OFFSET))
#define SHM_SIZE(ptr) (*(size_t *)SHM_HEAD_PTR(ptr))


void *akm_shm_alloc(size_t size)
{
    void *ptr;
    size += SHM_OFFSET; // first byte store shm size
    size = AKM_SMM_ALIGNED_SIZE(size); // memory aligned

    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        printf("cannot alloc shm:%d (%d)\n", __LINE__, errno);
        return NULL;
    }

    ptr = ((unsigned char *)ptr) + SHM_OFFSET;
    SHM_SIZE(ptr) = size;
    return ptr;
}

void *akm_shm_realloc(void *ptr, size_t size)
{
    if (SHM_SIZE(ptr) == size) {
        return ptr;
    }
    size_t copy_size = SHM_SIZE(ptr) < size ? SHM_SIZE(ptr) : size;
    void *newptr = akm_shm_alloc(size);
    if (newptr == NULL) {
        return NULL;
    }
    memcpy(newptr, ptr, copy_size);
    akm_shm_free(ptr);
    return newptr;
}

void akm_shm_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    if (SHM_SIZE(ptr))
        munmap(SHM_HEAD_PTR(ptr), SHM_SIZE(ptr));
}
