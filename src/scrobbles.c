/*
 * Licensed under BSD 2-Clause License
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_scrobbles.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

int
mstdnt_scrobble_json(struct mstdnt_scrobble* scrobble, cJSON* js)
{
    cJSON* v;

    /* Zero out */
    memset(scrobble, 0, sizeof(struct mstdnt_scrobble));

    struct _mstdnt_val_ref vals[] = {
        { "account", &(scrobble->account), _mstdnt_val_account_call },
        { "album", &(scrobble->album), _mstdnt_val_string_call },
        { "artist", &(scrobble->artist), _mstdnt_val_string_call },
        { "created_at", &(scrobble->created_at), _mstdnt_val_datetime_unix_call },
        { "id", &(scrobble->id), _mstdnt_val_string_call },
        { "length", &(scrobble->length), _mstdnt_val_uint_call },
        { "title", &(scrobble->title), _mstdnt_val_string_call }
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

GENERATE_JSON_ARRAY_FUNC(mstdnt_scrobbles_json, struct mstdnt_scrobble, mstdnt_scrobble_json)
    
int
mstdnt_scrobbles_json_callback(cJSON* json,
                               void* _args,
                               mstdnt_request_cb_data* data)
{
    mstdnt_scrobbles* scrobbles = mstdnt_malloc(sizeof(mstdnt_scrobbles));
    data->data = scrobbles;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_scrobbles;
    return mstdnt_scrobbles_json(&(scrobbles->scrobbles), &(scrobbles->len), json);
}

int
mstdnt_get_scrobbles(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* id,
                     struct mstdnt_get_scrobbles_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/accounts/%s/scrobbles", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
    };

    struct mstdnt_request_args req_args = {
        .url = url,
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_scrobbles_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void
mstdnt_cleanup_scrobble(mstdnt_scrobble* scrobble)
{
    mstdnt_cleanup_account(&(scrobble->account));
}

void
mstdnt_cleanup_scrobbles(mstdnt_scrobbles* scrobbles)
{
    if (!scrobbles) return;
    for (size_t i = 0; i < scrobbles->len; ++i)
    {
        mstdnt_cleanup_scrobble(scrobbles->scrobbles + i);
    }
    mstdnt_free(scrobbles->scrobbles);
}
