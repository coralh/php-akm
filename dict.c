#include "dict.h"
#include "shm.h"

akm_dict_t *akm_dict_new(char *filename)
{
    akm_dict_t *dict = akm_shm_alloc(sizeof(akm_dict_t));
    if (dict == NULL) {
        return NULL;
    }
    memset(dict, 0, sizeof(akm_dict_t));
}

void akm_dict_free(akm_dict_t *dict)
{

}
