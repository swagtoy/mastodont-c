/*
 * Licensed under BSD 3-Clause License
 */

#include <string.h>
#include <mastodont_hooks.h>
#include <mastodont_history.h>
#include <mastodont_json_helper.h>
#include <mastodont_generate.h>

int mstdnt_history_json(struct mstdnt_history* hist, cJSON* js)
{
    memset(hist, 0, sizeof(struct mstdnt_history));

    struct _mstdnt_val_ref refs[] = {
        { "day", &(hist->day), _mstdnt_val_string_unix_call },
        { "uses", &(hist->uses), _mstdnt_val_string_uint_call },
        { "accounts", &(hist->accounts), _mstdnt_val_string_uint_call },
    };

    for (cJSON* v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    
    return 0;
}

// Generated function
GENERATE_JSON_ARRAY_FUNC(mstdnt_histories_json, struct mstdnt_history, mstdnt_history_json)

void _mstdnt_val_histories_call(cJSON* v, void* _type)
{
    struct _mstdnt_generic_args* args = _type;
    struct mstdnt_history** hist = args->arg;
    cJSON* v_array = v->child;
    cJSON* att = NULL;

    size_t size = cJSON_GetArraySize(v);
    *(args->size) = size;
    /* No attachments, ignore */
    if (size == 0)
    {
        *hist = NULL;
        return;
    }

    *hist = mstdnt_calloc(1, sizeof(struct mstdnt_history) * size);
    if (*hist == NULL)
        return;

    cJSON* it;
    int i;
    for (it = v_array, i = 0; it; (++i, it = it->next))
    {
        mstdnt_history_json((*hist) + i, it->child);
    }    
}
