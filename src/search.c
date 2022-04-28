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

#include <mastodont_search.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

static const char* type_to_string(enum mstdnt_search_type type)
{
    switch (type)
    {
    case MSTDNT_SEARCH_ACCOUNTS: return "accounts";
    case MSTDNT_SEARCH_HASHTAGS: return "hashtags";
    case MSTDNT_SEARCH_STATUSES: return "statuses";
    default: return NULL;
    }
}

int mastodont_search(mastodont* data,
                     char* query,
                     struct mstdnt_storage* storage,
                     struct mstdnt_search_args* args,
                     struct mstdnt_search_results* results)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERR_STRING, "q", { .s = query } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "type", { .s = type_to_string(args->type) } },
        { _MSTDNT_QUERY_INT, "resolve", { .i = args->resolve } },
        { _MSTDNT_QUERY_INT, "following", { .i = args->following } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args->with_relationships } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
    };

    struct mastodont_request_args req_args = {
        storage,
        "api/v2/search",
        params, _mstdnt_arr_len(parma),
        NULL, 0,
        CURLOPT_HTTPGET,
        results,
        _mstdnt_search_result_callback,
    };
    
    return mastodont_request(data, &req_args);
}
