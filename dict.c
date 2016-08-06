#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_akm.h"
#include "dict.h"
#include "shm.h"

#ifndef USING_SHM
#define akm_shm_alloc malloc
#define akm_shm_free free
#endif

static inline void akm_build_node(akm_trie_t *trie, char *keyword,
        size_t keyword_len, char *extension)
{
    akm_pattern_t patt;

    /* Fill the pattern data */
    patt.ptext.astring = keyword;
    patt.ptext.length  = keyword_len;

    /* The replacement pattern is not applicable in this program, so better
     * to initialize it with 0 */
    patt.rtext.astring = NULL;
    patt.rtext.length  = 0;

    patt.id.u.stringy = extension;
    patt.id.type      = AKM_PATTID_TYPE_STRING;

    /* Add pattern to automata */
    akm_trie_add (trie, &patt, 1);
}

static void akm_build_tree(akm_trie_t *trie, char *fullpath)
{
    php_stream *stream;
    char       *line;
    size_t      len = 0, i;

    char       *keyword,
               *extension;
    size_t      keyword_len;

    struct stat st;

    stat(fullpath, &st);
    if (st.st_size == 0) {
        return;
    }

    stream = php_stream_open_wrapper(fullpath, "r", REPORT_ERRORS, NULL);
    if (!stream) {
        return;
    }

    while (NULL != (line = php_stream_get_line(stream, NULL, 0, &len))) {
        /* remove \r\n */
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line[len - 2] == '\r') {
            line[len - 2] = '\0';
        }
        len = strlen(line);

        if (len == 0 || line[0] == DELIMITER) {
            continue;
        }

        /* find delimiter */
        keyword     = line;
        keyword_len = 0;
        extension   = NULL;
        for (i = 0; i < len; i++) {
            if (line[i] == DELIMITER) {
                keyword_len = i;
                break;
            }
        }

        if (keyword_len == 0) { /* not found */
            keyword_len = len;
        } else {
            if (keyword_len + 1 == len) { /* example: "keyword|" */
                keyword_len = len - 1;
            } else {
                extension = keyword + keyword_len + 1;
            }
        }

        akm_build_node(trie, keyword, keyword_len, extension);
        efree(line);
    }

    akm_trie_finalize (trie);
}


akm_dict_t *akm_dict_new(char *dict_name, char *fullpath)
{
    akm_dict_t *dict = akm_shm_alloc(sizeof(akm_dict_t));
    if (dict == NULL) {
        return NULL;
    }
    memset(dict, 0, sizeof(akm_dict_t));

    dict->trie = akm_trie_create();
    strcpy(dict->dict_name, dict_name);
    dict->is_free = 0;
    dict->ref_count = 0;
    akm_build_tree(dict->trie, fullpath);
    return dict;
}

void akm_dict_free(akm_dict_t *dict)
{
    akm_trie_release (dict->trie);
    akm_shm_free(dict);
}

akm_dict_t *akm_dict_find(struct list_head *list, char *dict_name)
{
    akm_dict_t *dict;
    struct list_head *e, *n;

    list_for_each_safe(e, n, list) {
        dict = list_entry(e, akm_dict_t, link);
        if (strcmp(dict->dict_name, dict_name) == 0) {
            return dict;
        }
    }
    return NULL;
}
