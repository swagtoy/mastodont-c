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
#include <mastodont_tag.h>

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

int mstdnt_search_json(struct mstdnt_search_results* search_results, cJSON* root)
{
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

    mstdnt_tags_json(&(search_results->tags),
                     &(search_results->tags_len),
                     hashtags);
    
    return 0;
}

int mstdnt_search_json_callback(cJSON* json, void* _args)
{
    return mstdnt_search_json(_args, json);
}

int mastodont_search(mastodont_t* data,
                     struct mstdnt_args* m_args,
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
        NULL,
        results,
        mstdnt_search_json_callback
    };
    
    return mastodont_request(data, m_args, &req_args);
}

void mstdnt_cleanup_search_results(struct mstdnt_search_results* res)
{
    if (!res) return;
    mstdnt_cleanup_accounts(res->accts, res->accts_len);
    mstdnt_cleanup_statuses(res->statuses, res->statuses_len);
    mstdnt_cleanup_tags(res->tags, res->tags_len);
}
