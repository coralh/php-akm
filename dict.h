#pragma once

#include "ahocorasick/ahocorasick.h"
#include <time.h>
#include "list.h"

#define DELIMITER '|'
#define MAX_DICT_NAME_LENGTH     (48)

#define AKM_DICT_REF(dict) (((akm_dict_t *)dict)->ref_count++)
#define AKM_DICT_UNREF(dict) (((akm_dict_t *)dict)->ref_count--)

typedef struct {
    akm_trie_t      *trie;
    char             dict_name[MAX_DICT_NAME_LENGTH];
    int              is_free:1;
    int              ref_count;
    time_t           mtime;
    struct list_head link;
} akm_dict_t;

akm_dict_t *akm_dict_new(char *dict_name, char *fullpath);
void akm_dict_free(akm_dict_t *dict);
akm_dict_t *akm_dict_find(struct list_head *list, char *filename);
