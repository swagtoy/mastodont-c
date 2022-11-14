/*
 * Licensed under BSD 3-Clause License
 */

#include <stdlib.h>
#include <mastodont_search.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_status.h>
#include <mastodont_account.h>
#include <mastodont_tag.h>

static const char*
type_to_string(enum mstdnt_search_type type)
{
    switch (type)
    {
    case MSTDNT_SEARCH_ACCOUNTS: return "accounts";
    case MSTDNT_SEARCH_HASHTAGS: return "hashtags";
    case MSTDNT_SEARCH_STATUSES: return "statuses";
    default: return NULL;
    }
}

int
mstdnt_search_json(struct mstdnt_search_results* search_results,
                   cJSON* root)
{
    // Not many items here, just use cJSON_GetObjectItemCaseSensitive() instead
    cJSON* statuses = cJSON_GetObjectItemCaseSensitive(root, "statuses");
    cJSON* accounts = cJSON_GetObjectItemCaseSensitive(root, "accounts");
    cJSON* hashtags = cJSON_GetObjectItemCaseSensitive(root, "hashtags");

    // Statuses
    mstdnt_statuses_json(&(search_results->statuses->statuses),
                         &(search_results->statuses->len),
                         statuses);

    mstdnt_accounts_json(&(search_results->accts->accts),
                         &(search_results->accts->len),
                         accounts);

    // TODO finish tags
    /* mstdnt_tags_json(&(search_results->tags->tags), */
    /*                  &(search_results->tags->len), */
    /*                  hashtags); */
    
    return 0;
}

int
mstdnt_search_json_callback(cJSON* json,
                            void* args,
                            mstdnt_request_cb_data* data)
{
    struct mstdnt_search_results* results = malloc(sizeof(struct mstdnt_search_results));
    data->data = results;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_search_results;
    return mstdnt_search_json(results, json);
}

int
mstdnt_search(mastodont_t* data,
              struct mstdnt_args* m_args,
              mstdnt_request_cb_t cb_request,
              void* cb_args,
              char* query,
              struct mstdnt_search_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "q", { .s = query } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "type", { .s = (char*)type_to_string(args.type) } },
        { _MSTDNT_QUERY_INT, "resolve", { .i = args.resolve } },
        { _MSTDNT_QUERY_INT, "following", { .i = args.following } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args.with_relationships } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
    };

    struct mstdnt_request_args req_args = {
        .url = "api/v2/search",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_search_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void
mstdnt_cleanup_search_results(struct mstdnt_search_results* res)
{
    if (!res) return;
    mstdnt_cleanup_accounts(res->accts);
    mstdnt_cleanup_statuses(res->statuses);
    // TODO
    //mstdnt_cleanup_tags(res->tags, res->tags_len);
}
