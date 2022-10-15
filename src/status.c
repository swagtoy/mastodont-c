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
#include <mastodont_hooks.h>
#include <mastodont_json_helper.h>
#include <mastodont_status.h>
#include <mastodont_account.h>
#include <mastodont_query.h>
#include <mastodont_pleroma.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

void _mstdnt_val_status_call(cJSON* v, void* _type)
{
    struct mstdnt_status* type = _type;

    mstdnt_status_json(type, v->child);
}


void _mstdnt_val_malloc_status_call(cJSON* v, void* _type)
{
    struct mstdnt_status** type = _type;

    /* Status value may be NULL */
    if (!(v->child))
        return;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_status));
    
    if (*type)
        mstdnt_status_json(*type, v->child);
}

// Consider moving to mstdnt_visibility_types?
static void _mstdnt_val_visibility_call(cJSON* v, void* _type)
{
    enum mstdnt_visibility_type* type = _type;

    // Check first, as we must read it
    if (!cJSON_IsString(v))
    {
        *type = MSTDNT_VISIBILITY_UNKNOWN;
        return;
    }

    char* str = v->valuestring;
    if (strcmp(str, "public") == 0)
        *type = MSTDNT_VISIBILITY_PUBLIC;
    else if (strcmp(str, "unlisted") == 0)
        *type = MSTDNT_VISIBILITY_UNLISTED;
    else if (strcmp(str, "private") == 0)
        *type = MSTDNT_VISIBILITY_PRIVATE;
    else if (strcmp(str, "direct") == 0)
        *type = MSTDNT_VISIBILITY_DIRECT;
    else if (strcmp(str, "local") == 0)
        *type = MSTDNT_VISIBILITY_LOCAL;
    else if (strcmp(str, "list") == 0)
        *type = MSTDNT_VISIBILITY_LIST;
    else
        *type = MSTDNT_VISIBILITY_UNKNOWN;
}

int mstdnt_status_json(struct mstdnt_status* status, cJSON* js)
{
    cJSON* v;

    // Not an error
    if (!(status && js))
        return 0;

    /* Zero out */
    memset(status, 0, sizeof(struct mstdnt_status));

    struct _mstdnt_generic_args att_args = {
        &(status->media_attachments),
        &(status->media_attachments_len),
    };

    struct _mstdnt_generic_args emj_args = {
        &(status->emojis),
        &(status->emojis_len)
    };

    struct _mstdnt_val_ref vals[] = {
        { "id", &(status->id), _mstdnt_val_string_call },
        { "uri", &(status->uri), _mstdnt_val_string_call },
        { "created_at", &(status->created_at), _mstdnt_val_datetime_unix_call },
        { "content", &(status->content), _mstdnt_val_string_call },
        { "spoiler_text", &(status->spoiler_text), _mstdnt_val_string_call },
        { "in_reply_to_id", &(status->in_reply_to_id), _mstdnt_val_string_call },
        { "language", &(status->language), _mstdnt_val_string_call },
        { "url", &(status->url), _mstdnt_val_string_call },
        { "text", &(status->text), _mstdnt_val_string_call },
        { "visibility", &(status->visibility), _mstdnt_val_visibility_call },
        { "in_reply_to_account_id", &(status->in_reply_to_account_id), _mstdnt_val_string_call },
        { "sensitive", &(status->sensitive), _mstdnt_val_bool_call },
        { "favourited", &(status->favourited), _mstdnt_val_bool_call },
        { "reblogged", &(status->reblogged), _mstdnt_val_bool_call },
        { "muted", &(status->muted), _mstdnt_val_bool_call },
        { "emojis", &emj_args, _mstdnt_val_emojis_call },
        { "bookmarked", &(status->bookmarked), _mstdnt_val_bool_call },
        { "pinned", &(status->pinned), _mstdnt_val_bool_call },
        { "reblogs_count", &(status->reblogs_count), _mstdnt_val_uint_call },
        { "application", &(status->application), _mstdnt_val_malloc_application_call },
        { "favourites_count", &(status->favourites_count), _mstdnt_val_uint_call },
        { "replies_count", &(status->replies_count), _mstdnt_val_uint_call },
        { "media_attachments", &att_args, _mstdnt_val_attachments_call },
        { "pleroma", &(status->pleroma), _mstdnt_val_status_pleroma_call },
        { "reblog", &(status->reblog), _mstdnt_val_malloc_status_call },
        { "account", &(status->account), _mstdnt_val_account_call }
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

int mstdnt_status_json_callback(cJSON* json, void* status)
{
    return mstdnt_status_json((struct mstdnt_status*)status, json->child);
}

// GENERATE mstdnt_statuses_json
GENERATE_JSON_ARRAY_FUNC(mstdnt_statuses_json, struct mstdnt_status, mstdnt_status_json)

int mstdnt_statuses_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_statuses_cb_args* args = _args;
    return mstdnt_statuses_json(args->statuses, args->size, json);
}

int mstdnt_get_account_statuses(mstdnt_t* data,
                                   struct mstdnt_args* m_args,
                                   char* id,
                                   struct mstdnt_account_statuses_args* args,
                                   struct mstdnt_storage* storage,
                                   struct mstdnt_status* statuses[],
                                   size_t* size)
{
    char url[MSTDNT_URLSIZE];
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/statuses", id);

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_INT, "pinned", { .i = args->pinned } },
        { _MSTDNT_QUERY_STRING, "tagged", { .s = args->tagged } },
        { _MSTDNT_QUERY_INT, "only_media", { .i = args->only_media } },
        { _MSTDNT_QUERY_INT, "with_muted", { .i = args->with_muted } },
        { _MSTDNT_QUERY_INT, "exclude_reblogs", { .i = args->exclude_reblogs } },
        { _MSTDNT_QUERY_INT, "exclude_replies", { .i = args->exclude_replies } },
        { _MSTDNT_QUERY_STRING, "max_id",  { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
    };

    struct mstdnt_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &cb_args,
        mstdnt_statuses_json_callback
    };
    
    return mstdnt_request(data, m_args, &req_args);
}

/* TODO Populate the arguments! */
int mstdnt_create_status(mstdnt_t* data,
                            struct mstdnt_args* m_args,
                            struct mstdnt_status_args* args,
                            struct mstdnt_storage* storage)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "in_reply_to_id", { .s = args->in_reply_to_id } },
        { _MSTDNT_QUERY_STRING, "content_type", { .s = args->content_type } },
        { _MSTDNT_QUERY_STRING, "status", { .s = args->status } },
        { _MSTDNT_QUERY_STRING, "visibility", { .s = args->visibility } },
        { _MSTDNT_QUERY_ARRAY, "media_ids",
          {
              .a.arr = args->media_ids,
              .a.arr_len = args->media_ids_len
          }
        },
    };

    struct mstdnt_request_args req_args = {
        storage,
        "api/v1/statuses",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        NULL,
        NULL, /* TODO populate the status back?
               * (not sure if the api returns it or not) */
    };

    return mstdnt_request(data, m_args, &req_args);
}

static int mstdnt_status_action(mstdnt_t* data,
                                struct mstdnt_args* m_args,
                                char* id,
                                struct mstdnt_storage* storage,
                                struct mstdnt_status* status,
                                char* url_str)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, url_str, id);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_POST,
        NULL,
        status,
        mstdnt_status_json_callback
    };

    return mstdnt_request(data, m_args, &req_args);
}

/* These are all the same */
MSTDNT_STATUS_ACTION_DECL(favourite)
    MSTDNT_STATUS_ACTION_FUNC_URL("favourite")

    MSTDNT_STATUS_ACTION_DECL(unfavourite)
    MSTDNT_STATUS_ACTION_FUNC_URL("unfavourite")

    MSTDNT_STATUS_ACTION_DECL(reblog)
    MSTDNT_STATUS_ACTION_FUNC_URL("reblog")

    MSTDNT_STATUS_ACTION_DECL(unreblog)
    MSTDNT_STATUS_ACTION_FUNC_URL("unreblog")
    
    MSTDNT_STATUS_ACTION_DECL(pin)
    MSTDNT_STATUS_ACTION_FUNC_URL("pin")

    MSTDNT_STATUS_ACTION_DECL(unpin)
    MSTDNT_STATUS_ACTION_FUNC_URL("unpin")

    MSTDNT_STATUS_ACTION_DECL(bookmark)
    MSTDNT_STATUS_ACTION_FUNC_URL("bookmark")

    MSTDNT_STATUS_ACTION_DECL(unbookmark)
MSTDNT_STATUS_ACTION_FUNC_URL("unbookmark")

// Delete's use a delete method
MSTDNT_STATUS_ACTION_DECL(delete)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s", id);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_CUSTOMREQUEST,
        "DELETE",
        status,
        mstdnt_status_json_callback
    };

    return mstdnt_request(data, m_args, &req_args);
}

/* TODO Mutes can be timed */
int mstdnt_mute_conversation(mstdnt_t* data,
                                struct mstdnt_args* m_args,
                                char* id,
                                struct mstdnt_storage* storage,
                                struct mstdnt_status* status)
{
    return mstdnt_status_action(data, m_args, id, storage, status, "api/v1/statuses/%s/mute");
}

int mstdnt_unmute_conversation(mstdnt_t* data,
                                  struct mstdnt_args* m_args,
                                  char* id,
                                  struct mstdnt_storage* storage,
                                  struct mstdnt_status* status)
{
    return mstdnt_status_action(data, m_args, id, storage, status, "api/v1/statuses/%s/unmute");
}

int mstdnt_get_status(mstdnt_t* data,
                         struct mstdnt_args* m_args,
                         char* id,
                         struct mstdnt_storage* storage,
                         struct mstdnt_status* status)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s", id);
    
    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        status,
        mstdnt_status_json_callback,
    };

    return mstdnt_request(data, m_args, &req_args);
}

int mstdnt_status_context_json(struct mstdnt_status* statuses_before[],
                               struct mstdnt_status* statuses_after[],
                               size_t* size_before,
                               size_t* size_after,
                               cJSON* root)
{
    cJSON* v, *status_item;
    size_t* size_ptr = NULL;
    struct mstdnt_status** stat_ptr = NULL;

    /* Empty data */
    *size_before = 0;
    *size_after = 0;
    *statuses_before = NULL;
    *statuses_after = NULL;
    
    for (v = root->child; v; v = v->next)
    {
        if (cJSON_IsObject(v->child))
        {
            /* Set pointers */
            if (strcmp(v->string, "ancestors") == 0)
            {
                stat_ptr = statuses_before;
                size_ptr = size_before;
            }
            if (strcmp(v->string, "descendants") == 0) {
                stat_ptr = statuses_after;
                size_ptr = size_after;
            }

            /* Skip allocating empty arrays */
            if (cJSON_GetArraySize(v) <= 0)
                continue;

            *stat_ptr = mstdnt_calloc(1, cJSON_GetArraySize(v) * sizeof(struct mstdnt_status));
            if (*stat_ptr == NULL)
                return 1;
            
            cJSON_ArrayForEach(status_item, v)
            {
                mstdnt_status_json((*stat_ptr) + (*size_ptr)++, status_item->child);
            }
        }
    }
    
    return 0;
}

int mstdnt_status_context_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_status_context_result_cb_args* args = _args;
    return mstdnt_status_context_json(args->statuses_before,
                                      args->statuses_after,
                                      args->size_before,
                                      args->size_after,
                                      json);
}

int mstdnt_get_status_context(mstdnt_t* data,
                                 struct mstdnt_args* m_args,
                                 char* id,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_status* statuses_before[],
                                 struct mstdnt_status* statuses_after[],
                                 size_t* size_before,
                                 size_t* size_after)
{
    struct _mstdnt_status_context_result_cb_args args = {
        statuses_before,
        statuses_after,
        size_before,
        size_after,
    };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s/context", id);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &args,
        mstdnt_status_context_json_callback,
    };

    return mstdnt_request(data, m_args, &req_args);
}

int mstdnt_status_favourited_by(mstdnt_t* data,
                                   struct mstdnt_args* m_args,
                                   char* id,
                                   struct mstdnt_storage* storage,
                                   struct mstdnt_account* accounts[],
                                   size_t* accts_len)
{
    struct _mstdnt_accounts_args args = {
        accounts,
        accts_len,
    };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s/favourited_by", id);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &args,
        mstdnt_accounts_json_callback,
    };

    return mstdnt_request(data, m_args, &req_args);
}

int mstdnt_status_reblogged_by(mstdnt_t* data,
                                  struct mstdnt_args* m_args,
                                  char* id,
                                  struct mstdnt_storage* storage,
                                  struct mstdnt_account* accounts[],
                                  size_t* accts_len)
{
    struct _mstdnt_accounts_args args = {
        accounts,
        accts_len,
    };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s/reblogged_by", id);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &args,
        mstdnt_accounts_json_callback,
    };

    return mstdnt_request(data, m_args, &req_args);
}


int mstdnt_get_bookmarks(mstdnt_t* data,
                            struct mstdnt_args* m_args,
                            struct mstdnt_bookmarks_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
    };
    
    struct mstdnt_request_args req_args = {
        storage,
        "api/v1/bookmarks",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &cb_args,
        mstdnt_statuses_json_callback,
    };
    
    return mstdnt_request(data, m_args, &req_args);
}

int mstdnt_get_favourites(mstdnt_t* data,
                             struct mstdnt_args* m_args,
                             struct mstdnt_favourites_args* args,
                             struct mstdnt_storage* storage,
                             struct mstdnt_status* statuses[],
                             size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
    };
    
    struct mstdnt_request_args req_args = {
        storage,
        "api/v1/favourites",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &cb_args,
        mstdnt_statuses_json_callback,
    };
    
    return mstdnt_request(data, m_args, &req_args);
}

int mstdnt_status_emoji_react(mstdnt_t* api,
                                 struct mstdnt_args* m_args,
                                 char* id,
                                 char* emoji,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_status* status)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/statuses/%s/reactions/%s", id, emoji);

    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_PUT,
        NULL,
        status,
        mstdnt_status_json_callback
    };
    
    return mstdnt_request(api, m_args, &req_args);
}

void mstdnt_cleanup_status(struct mstdnt_status* status)
{
    mstdnt_cleanup_attachments(status->media_attachments);
    mstdnt_cleanup_account(&(status->account));
    mstdnt_cleanup_status_pleroma(&(status->pleroma));
    mstdnt_cleanup_emojis(status->emojis);
    if (status->reblog)
    {
        mstdnt_cleanup_status(status->reblog);
        mstdnt_free(status->reblog);
    }
    mstdnt_free(status->application);
}

void mstdnt_cleanup_statuses(struct mstdnt_status* statuses, size_t s)
{
    size_t i;
    if (!statuses) return;
    for (i = 0; i < s; ++i)
    {
        mstdnt_cleanup_status(statuses + i);
    }
    mstdnt_free(statuses);
}
