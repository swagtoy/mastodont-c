/*
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_scrobbles.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

int mstdnt_scrobble_json(struct mstdnt_scrobble* scrobble, cJSON* js)
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
    
int mstdnt_scrobbles_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_scrobbles_cb_args* args = _args;
    return mstdnt_scrobbles_json(args->scrobbles, args->size, json);
}

int mstdnt_get_scrobbles(mastodont_t* data,
                            struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                            char* id,
                            struct mstdnt_get_scrobbles_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_scrobble* scrobbles[],
                            size_t* size)
{
    struct _mstdnt_scrobbles_cb_args cb_args = { scrobbles, size };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/accounts/%s/scrobbles", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
    };

    struct mstdnt_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &cb_args,
        mstdnt_scrobbles_json_callback
    };

    return mstdnt_request(data, m_args, &req_args);
}

