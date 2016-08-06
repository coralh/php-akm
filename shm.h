#pragma once

#include <stddef.h>

#define AKM_SMM_ALIGNMENT           8
#define AKM_SMM_ALIGNMENT_MASK      ~(AKM_SMM_ALIGNMENT - 1)
#define AKM_SMM_ALIGNED_SIZE(x)     (((x) + AKM_SMM_ALIGNMENT - 1) & AKM_SMM_ALIGNMENT_MASK)

void *akm_shm_alloc(size_t size);
void *akm_shm_realloc(void *ptr, size_t size);
void akm_shm_free(void *ptr);
