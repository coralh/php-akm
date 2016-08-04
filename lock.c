#include "lock.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int akm_lock_init(akm_lock_t *l)
{
    pthread_mutexattr_t mattr;
    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        return -1;
    }

    l->mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
            MAP_SHARED, fd, 0);
    if (l->mutex == MAP_FAILED) {
        return -1;
    }
    close(fd);

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(l->mutex, &mattr);
    return 0;
}

void akm_lock_lock(akm_lock_t *l)
{
    pthread_mutex_lock(l->mutex);
}

void akm_lock_unlock(akm_lock_t *l)
{
    pthread_mutex_unlock(l->mutex);
}

void akm_lock_free(akm_lock_t *l)
{
    pthread_mutex_destroy(l->mutex);
    munmap(l->mutex, sizeof(pthread_mutex_t));
}
