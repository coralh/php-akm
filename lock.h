#pragma once
#include <pthread.h>

typedef struct akm_lock_s akm_lock_t;

struct akm_lock_s {
    pthread_mutex_t *mutex;
};

int akm_lock_init(akm_lock_t *l);
void akm_lock_lock(akm_lock_t *l);
void akm_lock_unlock(akm_lock_t *l);
void akm_lock_free(akm_lock_t *l);
