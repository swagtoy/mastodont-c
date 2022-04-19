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
#include <mastodont_json_helper.h>
#include <mastodont_status.h>
#include <mastodont_account.h>
#include <mastodont_query.h>
#include <mastodont_pleroma.h>
#include <mastodont_request.h>

void _mstdnt_val_status_call(cJSON* v, void* _type)
{
    struct mstdnt_status* type = _type;

    mstdnt_status_from_json(type, v->child);
}


void _mstdnt_val_malloc_status_call(cJSON* v, void* _type)
{
    struct mstdnt_status** type = _type;

    /* Status value may be NULL */
    if (!(v->child))
        return;

    *type = calloc(1, sizeof(struct mstdnt_status));
    
    if (*type)
        mstdnt_status_from_json(*type, v->child);
}

int mstdnt_status_from_json(struct mstdnt_status* status, cJSON* js)
{
    cJSON* v;

    /* Zero out */
    memset(status, 0, sizeof(struct mstdnt_status));

    struct _mstdnt_generic_args att_args = {
        &(status->media_attachments),
        &(status->media_attachments_len),
    };

    struct _mstdnt_val_ref vals[] = {
        { "id", &(status->id), _mstdnt_val_string_call },
        { "uri", &(status->uri), _mstdnt_val_string_call },
        { "created_at", &(status->created_at), _mstdnt_val_string_call },
        { "content", &(status->content), _mstdnt_val_string_call },
        { "spoiler_text", &(status->spoiler_text), _mstdnt_val_string_call },
        { "in_reply_to_id", &(status->in_reply_to_id), _mstdnt_val_string_call },
        { "language", &(status->language), _mstdnt_val_string_call },
        { "url", &(status->url), _mstdnt_val_string_call },
        { "text", &(status->text), _mstdnt_val_string_call },
        { "visibility", &(status->visibility), _mstdnt_val_string_call },
        { "in_reply_to_account_id", &(status->in_reply_to_account_id), _mstdnt_val_string_call },
        { "sensitive", &(status->sensitive), _mstdnt_val_bool_call },
        { "favourited", &(status->favourited), _mstdnt_val_bool_call },
        { "reblogged", &(status->reblogged), _mstdnt_val_bool_call },
        { "muted", &(status->muted), _mstdnt_val_bool_call },
        { "bookmarked", &(status->bookmarked), _mstdnt_val_bool_call },
        { "pinned", &(status->pinned), _mstdnt_val_bool_call },
        { "reblogs_count", &(status->reblogs_count), _mstdnt_val_uint_call },
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

int mstdnt_status_from_result(struct mstdnt_fetch_results* results,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* status)
{
    /* Can be null sometimes */
    if (!status) return 0;
    
    cJSON* root;
    if (_mstdnt_json_init(&root, results, storage) ||
        !cJSON_IsObject(root))
        return 1;

    return mstdnt_status_from_json(status, root->child);
}

int _mstdnt_status_from_result_callback(struct mstdnt_fetch_results* results,
                                        struct mstdnt_storage* storage,
                                        void* status)
{
    return mstdnt_status_from_result(results, storage, (struct mstdnt_status*)status);
}

int mstdnt_statuses_from_result(struct mstdnt_storage* storage,
                                struct mstdnt_fetch_results* results,
                                struct mstdnt_status* statuses[],
                                size_t* size)
{
    size_t i = 0;
    cJSON* root, *status_j_list;
    if (_mstdnt_json_init(&root, results, storage) &&
        !cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* Statuses can be an empty array! */
    if (!(size ? *size : cJSON_GetArraySize(root)))
        return 0; /* Not an error, but we are done parsing */

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *statuses = calloc(1, (size ? *size : cJSON_GetArraySize(root))
                       * sizeof(struct mstdnt_status));
    if (*statuses == NULL)
        return 1;
    
    cJSON_ArrayForEach(status_j_list, root)
    {
        mstdnt_status_from_json((*statuses) + i++, status_j_list->child);
    }
    
    return 0;
}

int _mstdnt_statuses_result_callback(struct mstdnt_fetch_results* results,
                                     struct mstdnt_storage* storage,
                                     void* _args)
{
    struct _mstdnt_statuses_cb_args* args = _args;
    return mstdnt_statuses_from_result(storage, results, args->statuses, args->size);
}

/* TODO Still need to handle get params */
int mastodont_get_account_statuses(mastodont_t* data,
                                   char* id,
                                   struct mstdnt_args* args,
                                   struct mstdnt_storage* storage,
                                   struct mstdnt_status* statuses[],
                                   size_t* size)
{
    char url[MSTDNT_URLSIZE];
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/statuses", id);

    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0, /* TODO */
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback
    };
    
    return mastodont_request(data, &req_args);
}

/* TODO Populate the arguments! */
int mastodont_create_status(mastodont_t* data,
                            struct mstdnt_args* args,
                            struct mstdnt_storage* storage)
{
    union param_value u_content_type, u_expires_in,
        u_in_reply_to_conversation_id, u_in_reply_to_id,
        u_language, u_media_ids, u_poll, u_preview, u_scheduled_at,
        u_sensitive, u_spoiler_text, u_status, u_visibility;
    u_content_type.s = args->content_type;
    u_expires_in.i = args->expires_in;
    u_in_reply_to_conversation_id.s = args->in_reply_to_conversation_id;
    u_in_reply_to_id.s = args->in_reply_to_id;
    u_language.s = args->language;
    u_media_ids.a.arr = args->media_ids;
    u_media_ids.a.arr_len = args->media_ids_len;
    /* poll */
    u_preview.i = args->preview;
    u_scheduled_at.s = args->scheduled_at;
    u_sensitive.i = args->sensitive;
    u_spoiler_text.s = args->spoiler_text;
    u_status.s = args->status;
    u_visibility.s = args->visibility;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "in_reply_to_id", u_in_reply_to_id },
        { _MSTDNT_QUERY_STRING, "content_type", u_content_type },
        { _MSTDNT_QUERY_STRING, "status", u_status },
        { _MSTDNT_QUERY_STRING, "visibility", u_visibility },
        { _MSTDNT_QUERY_ARRAY, "media_ids", u_media_ids },
    };

    struct mastodont_request_args req_args = {
        storage,
        "api/v1/statuses",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        NULL, /* TODO populate the status back?
               * (not sure if the api returns it or not) */
    };

    return mastodont_request(data, &req_args);
}

static int mstdnt_status_action(mastodont_t* data,
                                char* id,
                                struct mstdnt_storage* storage,
                                struct mstdnt_status* status,
                                char* url_str)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, url_str, id);

    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_POST,
        status,
        _mstdnt_status_from_result_callback
    };

    return mastodont_request(data, &req_args);
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

/* TODO Mutes can be timed */
int mastodont_mute_conversation(mastodont_t* data,
                                char* id,
                                struct mstdnt_storage* storage,
                                struct mstdnt_status* status)
{
    return mstdnt_status_action(data, id, storage, status, "api/v1/statuses/%s/mute");
}

int mastodont_unmute_conversation(mastodont_t* data,
                                  char* id,
                                  struct mstdnt_storage* storage,
                                  struct mstdnt_status* status)
{
    return mstdnt_status_action(data, id, storage, status, "api/v1/statuses/%s/unmute");
}

int mastodont_get_status(mastodont_t* data,
                         char* id,
                         struct mstdnt_storage* storage,
                         struct mstdnt_status* status)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s", id);
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        status,
        _mstdnt_status_from_result_callback,
    };

    return mastodont_request(data, &req_args);
}


int mstdnt_status_context_from_result(struct mstdnt_fetch_results* results,
                                      struct mstdnt_storage* storage,
                                      struct mstdnt_status* statuses_before[],
                                      struct mstdnt_status* statuses_after[],
                                      size_t* size_before,
                                      size_t* size_after)
{
    cJSON* root;

    /* Empty data */
    *size_before = 0;
    *size_after = 0;
    *statuses_before = NULL;
    *statuses_after = NULL;
    
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    mstdnt_status_context_from_json(results,
                                    storage,
                                    statuses_before,
                                    statuses_after,
                                    size_before,
                                    size_after,
                                    root);
    return 0;
}

int mstdnt_status_context_from_json(struct mstdnt_fetch_results* results,
                                    struct mstdnt_storage* storage,
                                    struct mstdnt_status* statuses_before[],
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

            *stat_ptr = calloc(1, cJSON_GetArraySize(v) * sizeof(struct mstdnt_status));
            if (*stat_ptr == NULL)
                return 1;
            
            cJSON_ArrayForEach(status_item, v)
            {
                mstdnt_status_from_json((*stat_ptr) + (*size_ptr)++, status_item->child);
            }
        }
    }
}

int _mstdnt_status_context_from_result_callback(struct mstdnt_fetch_results* results,
                                                struct mstdnt_storage* storage,
                                                void* _args)
{
    struct _mstdnt_status_context_result_cb_args* args = _args;
    return mstdnt_status_context_from_result(results,
                                             storage,
                                             args->statuses_before,
                                             args->statuses_after,
                                             args->size_before,
                                             args->size_after);
}

int mastodont_get_status_context(mastodont_t* data,
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

    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        &args,
        _mstdnt_status_context_from_result_callback,
    };

    return mastodont_request(data, &req_args);
}

void mstdnt_cleanup_status(struct mstdnt_status* status)
{
    cleanup_attachments(status->media_attachments);
    cleanup_status_pleroma(&(status->pleroma));
    if (status->reblog)
        free(status->reblog);
}

void mstdnt_cleanup_statuses(struct mstdnt_status* statuses, size_t s)
{
    size_t i;
    if (!statuses) return;
    for (i = 0; i < s; ++i)
    {
        mstdnt_cleanup_status(statuses + i);
    }
    free(statuses);
}
