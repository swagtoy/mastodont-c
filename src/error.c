/*
 * Licensed under BSD 2-Clause License
 */

#include <mastodont_query.h>
#include <mastodont_json_helper.h>

static void _mstdnt_val_errors_call(cJSON* v, void* _type)
{
    char** type = _type;
    if (v->child && v->child->valuestring)
        *type = v->child->valuestring;
    else
    {
        *type = NULL;
        return;
    }
}

int mstdnt_check_error(struct mstdnt_storage* storage)
{
    int res = 0;
    cJSON* v;
    cJSON* root = storage->root;

    /* Make sure empty */
    storage->error = NULL;
    storage->error_description = NULL;

    /* TODO I have no idea why multiple errors can be returned */
    struct _mstdnt_val_ref refs[] = {
        { "error", &(storage->error), _mstdnt_val_string_call },
        { "errors", &(storage->error), _mstdnt_val_errors_call },
        { "error_description", &(storage->error_description), _mstdnt_val_string_call },
    };
    
    for (v = root->child; v; v = v->next)
        if (_mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs)) == 0)
        {
            res = 1;
        }
    return res;
}
