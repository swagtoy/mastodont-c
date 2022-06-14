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
#include <mastodont_list.h>
#include <mastodont_json_helper.h>
#include <mastodont_fetch.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

struct mstdnt_get_lists_args {
    struct mstdnt_list** lists;
    size_t* size;
};

static void _mstdnt_val_replies_policy_call(cJSON* v, void* _type)
{
    enum mstdnt_list_replies_policy* type = _type;

    if (strcmp("none", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_NONE;
    else if (strcmp("list", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_LIST;
    else if (strcmp("followed", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_FOLLOWED;
}

int mstdnt_list_json(struct mstdnt_list* list, cJSON* js)
{
    if (!list) return 1;
    cJSON* v;

    /* Zero out */
    memset(list, 0, sizeof(struct mstdnt_list));
    
    struct _mstdnt_val_ref vals[] = {
        { "id", &(list->id), _mstdnt_val_string_call },
        { "title", &(list->title), _mstdnt_val_string_call },
        { "replies_policy", &(list->replies_policy), _mstdnt_val_replies_policy_call }
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}


GENERATE_JSON_ARRAY_FUNC(mstdnt_lists_json, struct mstdnt_list, mstdnt_list_json);

static int mstdnt_list_json_callback(cJSON* json, void* _args)
{
    return mstdnt_list_json(_args, json);
}

static int mstdnt_lists_json_callback(cJSON* json, void* _args)
{
    struct mstdnt_get_lists_args* args = _args;
    return mstdnt_lists_json(args->lists, args->size, json);
}

int mastodont_get_lists(mastodont_t* data,
                        struct mstdnt_storage* storage,
                        struct mstdnt_list* lists[],
                        size_t* size)
{
    struct mstdnt_get_lists_args args = {
        lists,
        size
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/lists",
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &args,
        mstdnt_lists_json_callback
    };

    return mastodont_request(data, &req_args);
}

int mastodont_get_list(mastodont_t* data,
                       char* id,
                       struct mstdnt_storage* storage,
                       struct mstdnt_list* list)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        list,
        mstdnt_list_json_callback
    };

    return mastodont_request(data, &req_args);
}

static const char* replies_policy_str(enum mstdnt_list_replies_policy pol)
{
    switch (pol)
    {
    case MSTDNT_LIST_REPLIES_POLICY_LIST:
        return "list";
    case MSTDNT_LIST_REPLIES_POLICY_FOLLOWED:
        return "followed";
    case MSTDNT_LIST_REPLIES_POLICY_NONE:
    default:
        return "none";
    }
}

int mastodont_create_list(mastodont_t* data,
                          struct mstdnt_list_args* args,
                          struct mstdnt_storage* storage,
                          struct mstdnt_list* list)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "title", { .s = args->title } },
        { _MSTDNT_QUERY_STRING, "replies_policy", { .s = (char*)replies_policy_str(args->replies_policy) } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/lists",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        list,
        mstdnt_list_json_callback
    };

    return mastodont_request(data, &req_args);
}

int mastodont_update_list(mastodont_t* data,
                          char* id,
                          struct mstdnt_list_args* args,
                          struct mstdnt_storage* storage,
                          struct mstdnt_list* list)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "title", { .s = args->title } },
        { _MSTDNT_QUERY_STRING, "replies_policy", { .s = (char*)replies_policy_str(args->replies_policy) } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_PUT,
        NULL,
        list,
        mstdnt_list_json_callback
    };

    return mastodont_request(data, &req_args);
}

int mastodont_delete_list(mastodont_t* api,
                          char* id,
                          struct mstdnt_storage* storage)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);

    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_CUSTOMREQUEST,
        "DELETE",
        NULL,
        NULL,
    };

    return mastodont_request(api, &req_args);
}

int mastodont_list_add_accounts(mastodont_t* api,
                                char* id,
                                char** account_ids,
                                size_t account_ids_len,
                                struct mstdnt_storage* storage)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s/accounts", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_ARRAY, "account_ids",
          {
              .a.arr = account_ids,
              .a.arr_len = account_ids_len
          }
        },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        NULL,
        NULL,
    };

    return mastodont_request(api, &req_args);
}

int mastodont_list_remove_accounts(mastodont_t* api,
                                   char* id,
                                   char** account_ids,
                                   size_t account_ids_len,
                                   struct mstdnt_storage* storage)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s/accounts", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_ARRAY, "account_ids",
          {
              .a.arr = account_ids,
              .a.arr_len = account_ids_len
          }
        },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_CUSTOMREQUEST,
        "DELETE",
        NULL,
        NULL,
    };

    return mastodont_request(api, &req_args);
}

int mastodont_list_get_accounts(mastodont_t* data,
                                char* id,
                                struct mstdnt_account_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_account* accts[],
                                size_t* accts_len)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s/accounts", id);
    struct _mstdnt_accounts_args _args = {
        accts,
        accts_len,
    };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args->with_relationships } },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &_args,
        mstdnt_accounts_json_callback,
    };

    return mastodont_request(data, &req_args);
}

void mstdnt_cleanup_lists(struct mstdnt_list* lists)
{
    free(lists);
}
