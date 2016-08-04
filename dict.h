#pragma once

#include "ahocorasick/ahocorasick.h"
#include <time.h>

typedef struct {
    akm_trie_t  *trie;
    char         filename[NAME_MAX];
    int          is_free:1;
    int          ref_count;
    time_t       mtime;
} akm_dict_t;

akm_dict_t *akm_dict_new(char *filename);
void akm_dict_free(akm_dict_t *dict);
