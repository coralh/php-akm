#pragma once

#define  AKM_HASH_OK              (0)
#define  AKM_HASH_ERR             (-1)
#define  AKM_HASH_DUPLICATE_KEY   (-2)

#define  AKM_HASH_BUCKETS_MIN_SIZE   7
#define  AKM_HASH_BUCKETS_MAX_SIZE   2147483647

typedef long akm_hash_hash_fn(char *, int);
typedef void akm_hash_free_fn(void *);
typedef void *akm_hash_malloc_fn(size_t);

typedef struct akm_hash_s {
    akm_hash_hash_fn *hash;
    akm_hash_free_fn *free;
    akm_hash_malloc_fn *malloc;
    void **buckets;
    unsigned int buckets_size;
    unsigned int elm_nums;
} akm_hash_t;

typedef struct akm_hash_entry_s akm_hash_entry_t;

struct akm_hash_entry_s {
    int hashval;
    int klen;
    void *data;
    akm_hash_entry_t *next;
    char key[0];
};


int akm_hash_init(akm_hash_t *o, unsigned int init_buckets, akm_hash_hash_fn *hash,
    akm_hash_free_fn *free, akm_hash_malloc_fn *mmalloc);
akm_hash_t *akm_hash_new(unsigned int init_buckets, akm_hash_hash_fn *hash,
    akm_hash_free_fn *free, akm_hash_malloc_fn *mmalloc);
int akm_hash_find(akm_hash_t *o, char *key, int klen, void **ret);
int akm_hash_insert(akm_hash_t *o, char *key, int klen, void *data, int replace);
int akm_hash_remove(akm_hash_t *o, char *key, int klen);
void akm_hash_destroy(akm_hash_t *o);
void akm_hash_free(akm_hash_t *o);
