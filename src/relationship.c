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

#include <mastodont_relationship.h>

struct _mstdnt_relationships_cb_args
{
    struct mstdnt_relationship** relationships;
    size_t* size;
};

int mstdnt_relationships_result(struct mstdnt_fetch_results* results,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships,
                                size_t* size)
{
}

int _mstdnt_relationships_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args)
{
    struct _mstdnt_relationships_cb_args* args = _args;
    return mstdnt_relationships_result(results, storage, args->relationships, args->size);
}

int mastodont_get_relationships(mastodont_t* data,
                                char** ids,
                                size_t ids_len,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships[],
                                size_t* size)
{
    /* struct _mstdnt_query_param params[] = { 0 }; */
    struct _mstdnt_relationships_cb_args cb_args = { relationships, size };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/accounts/relationships",
        NULL, 0,
        NULL, 0,
        /* params, _mstdnt_arr_len(params), */
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_relationships_result_callback, /* TODO populate the status back?
               * (not sure if the api returns it or not) */
    };

    return mastodont_request(data, &req_args);
}
