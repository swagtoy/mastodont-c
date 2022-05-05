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

#include <stdlib.h>
#include <mastodont_search.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_status.h>
#include <mastodont_account.h>

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

int mstdnt_search_from_result(struct mstdnt_storage* storage,
                              struct mstdnt_fetch_results* results,
                              struct mstdnt_search_results* search_results)
{
    cJSON* root;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    // Not many items here, just use cJSON_GetObjectItemCaseSensitive() instead
    cJSON* statuses = cJSON_GetObjectItemCaseSensitive(root, "statuses");
    cJSON* accounts = cJSON_GetObjectItemCaseSensitive(root, "accounts");
    cJSON* hashtags = cJSON_GetObjectItemCaseSensitive(root, "hashtags");

    // Statuses
    mstdnt_statuses_json(&(search_results->statuses),
                         &(search_results->statuses_len),
                         statuses);

    mstdnt_accounts_json(&(search_results->accts),
                         &(search_results->accts_len),
                         accounts);
    
    return 0;
}

int _mstdnt_search_result_callback(struct mstdnt_fetch_results* results,
                                   struct mstdnt_storage* storage,
                                   void* _args)
{
    return mstdnt_search_from_result(storage, results, _args);
}

int mastodont_search(mastodont_t* data,
                     char* query,
                     struct mstdnt_storage* storage,
                     struct mstdnt_search_args* args,
                     struct mstdnt_search_results* results)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "q", { .s = query } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "type", { .s = (char*)type_to_string(args->type) } },
        { _MSTDNT_QUERY_INT, "resolve", { .i = args->resolve } },
        { _MSTDNT_QUERY_INT, "following", { .i = args->following } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args->with_relationships } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
    };

    struct mastodont_request_args req_args = {
        storage,
        "api/v2/search",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        results,
        _mstdnt_search_result_callback
    };
    
    return mastodont_request(data, &req_args);
}
