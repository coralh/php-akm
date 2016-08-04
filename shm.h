#pragma once

#include <stddef.h>

void *akm_shm_alloc(size_t size);
void *akm_shm_realloc(void *ptr, size_t size);
void akm_shm_free(void *ptr);
