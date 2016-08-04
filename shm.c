#include "shm.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_OFFSET (sizeof(size_t))
#define SHM_HEAD_PTR(ptr) ((void *)(((unsigned char *)ptr) - SHM_OFFSET))
#define SHM_SIZE(ptr) (*(size_t *)SHM_HEAD_PTR(ptr))


void *akm_shm_alloc(size_t size)
{
    int fd;
    void *ptr;
    fd = open("/dev/zero", O_RDWR);
    if (fd == -1) {
        return NULL;
    }

    size += SHM_OFFSET; // first byte store shm size

    ptr = (void *)mmap(NULL, size, PROT_READ | PROT_WRITE,
            MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }

    ptr += SHM_OFFSET;
    SHM_SIZE(ptr) = size;

    close(fd);
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
    munmap(SHM_HEAD_PTR(ptr), SHM_SIZE(ptr));
}
