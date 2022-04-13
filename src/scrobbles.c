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

int mstdnt_scrobble_from_json(struct mstdnt_scrobble* scrobble, cJSON* js)
{
    cJSON* v;

    /* Zero out */
    memset(scrobble, 0, sizeof(struct mstdnt_scrobble));

    struct _mstdnt_val_ref vals[] = {
        { "account", &(scrobble->account), _mstdnt_val_account_call },
        { "album", &(scrobble->id), _mstdnt_val_string_call },
        { "artist", &(scrobble->id), _mstdnt_val_string_call },
        { "created_at", &(scrobble->id), _mstdnt_val_string_call },
        { "id", &(scrobble->id), _mstdnt_val_string_call },
        { "length", &(scrobble->id), _mstdnt_val_uint_call },
        { "title", &(scrobble->id), _mstdnt_val_string_call }
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

int mstdnt_scrobbles_result(struct mstdnt_fetch_results* results,
                            struct mstdnt_storage* storage,
                            struct mstdnt_scrobble* scrobbles[],
                            size_t* size)
{
    size_t i = 0;
    cJSON* root, *scrobble_j_list;
    if (_mstdnt_json_init(&root, results, storage) &&
        !cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* Scrobbles can be an empty array! */
    if (!(size ? *size : cJSON_GetArraySize(root)))
        return 0; /* Not an error, but we are done parsing */

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *scrobbles = calloc(1, (size ? *size : cJSON_GetArraySize(root))
                        * sizeof(struct mstdnt_scrobble));
    if (*scrobbles == NULL)
        return 1;
    
    cJSON_ArrayForEach(scrobble_j_list, root)
    {
        mstdnt_scrobble_json((*scrobbles) + i++, scrobble_j_list->child);
    }
    
    return 0;
}


int _mstdnt_scrobbles_from_result_callback(struct mstdnt_fetch_results* results,
                                           struct mstdnt_storage* storage,
                                           void* _args)
{
    struct _mstdnt_scrobbles_cb_args* args = _args;
    return mstdnt_scrobbles_from_result(results, storage, args->scrobbles, args->size);
}

int mastodont_get_scrobbles(mastodont_t* data,
                            char* id,
                            struct mstdnt_get_scrobbles_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_scrobble* scrobbles[],
                            size_t* size)
{
    struct _mstdnt_scrobbles_cb_args cb_args = { scrobbles, size };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/accounts/%s/scrobbles", id);
    
    union param_value u_max_id, u_min_id,
        u_since_id, u_offset, u_limit;
    u_max_id.s = args->max_id;
    u_min_id.s = args->min_id;
    u_since_id.s = args->since_id;
    u_offset.i = args->offset;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_scrobbles_from_result_callback
    };

    return mastodont_request(data, &req_args);
}

