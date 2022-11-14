/*
 * Licensed under BSD 3-Clause License
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_list.h>
#include <mastodont_json_helper.h>
#include <mastodont_fetch.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

struct mstdnt_get_lists_args {
    struct mstdnt_list** lists;
    size_t* size;
};

static void
_mstdnt_val_replies_policy_call(cJSON* v, void* _type)
{
    enum mstdnt_list_replies_policy* type = _type;

    if (strcmp("none", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_NONE;
    else if (strcmp("list", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_LIST;
    else if (strcmp("followed", v->valuestring) == 0)
        *type = MSTDNT_LIST_REPLIES_POLICY_FOLLOWED;
}

int
mstdnt_list_json(mstdnt_list* list, cJSON* js)
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

static int
mstdnt_list_json_callback(cJSON* json,
                          void* args,
                          mstdnt_request_cb_data* data)
{
    (void)args;
    mstdnt_list* list = mstdnt_malloc(sizeof(mstdnt_list));
    data->data = list;
    return mstdnt_list_json(list, json);
}

static int
mstdnt_lists_json_callback(cJSON* json,
                           void* args,
                           mstdnt_request_cb_data* data)
{
    (void)args;
    mstdnt_lists* lists = mstdnt_malloc(sizeof(mstdnt_lists));
    data->data = lists;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_lists;
    return mstdnt_lists_json(&(lists->lists), &(lists->len), json);
}

int
mstdnt_get_lists(mastodont_t* data,
                 struct mstdnt_args* m_args,
                 mstdnt_request_cb_t cb_request,
                 void* cb_args)
{
    struct mstdnt_request_args req_args = {
        .url = "api/v1/lists",
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_lists_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_get_list(mastodont_t* data,
                struct mstdnt_args* m_args,
                mstdnt_request_cb_t cb_request,
                void* cb_args,
                char* id)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_list_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

static const char*
replies_policy_str(enum mstdnt_list_replies_policy pol)
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

int
mstdnt_create_list(mastodont_t* data,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   struct mstdnt_list_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "title", { .s = args.title } },
        { _MSTDNT_QUERY_STRING, "replies_policy", { .s = replies_policy_str(args.replies_policy) } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/lists",
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_POST,
        .callback = mstdnt_list_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_update_list(mastodont_t* data,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   char* id,
                   struct mstdnt_list_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "title", { .s = args.title } },
        { _MSTDNT_QUERY_STRING, "replies_policy", { .s = (char*)replies_policy_str(args.replies_policy) } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_CUSTOMREQUEST,
        .request_type_custom = "PUT",
        .callback = mstdnt_list_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_delete_list(mastodont_t* api,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   char* id)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s", id);

    struct mstdnt_request_args req_args = {
        .url = url,
        .request_type = CURLOPT_CUSTOMREQUEST,
        .request_type_custom = "DELETE",
    };

    return mstdnt_request(api, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_list_add_accounts(mastodont_t* api,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* id,
                         char** account_ids,
                         size_t account_ids_len)
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
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_POST,
    };

    return mstdnt_request(api, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_list_remove_accounts(mastodont_t* api,
                            struct mstdnt_args* m_args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            char* id,
                            char** account_ids,
                            size_t account_ids_len)
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
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_CUSTOMREQUEST,
        .request_type_custom = "DELETE",
    };

    return mstdnt_request(api, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_list_get_accounts(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* id,
                         struct mstdnt_account_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/lists/%s/accounts", id);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args.with_relationships } },
    };

    struct mstdnt_request_args req_args = {
        .url = url,
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_accounts_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void mstdnt_cleanup_lists(struct mstdnt_lists* lists)
{
    mstdnt_free(lists->lists);
}
