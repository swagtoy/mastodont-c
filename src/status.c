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

int mstdnt_load_status_from_json(struct mstdnt_status* status, cJSON* js)
{
    cJSON* v;
    
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
        { "pleroma", &(status->pleroma), _mstdnt_val_status_pleroma_call }
    };
    
    for (v = js; v; v = v->next)
        if (_mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals)) == 1)
            if (cJSON_IsObject(v))
                if (strcmp("account", v->string) == 0)
                    mstdnt_load_account_from_json(&(status->account), v->child);
    return 0;
}

int mstdnt_load_status_from_result(struct mstdnt_status* status,
                                   struct mstdnt_storage* storage,
                                   struct mstdnt_fetch_results* results)
{
    cJSON* root;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    if (!cJSON_IsObject(root))
        return 1;

    if (!mstdnt_load_status_from_json(status, root->child))
        storage->needs_cleanup = 1;

    return 1;
}

int mstdnt_load_statuses_from_result(struct mstdnt_status* statuses[],
                                     struct mstdnt_storage* storage,
                                     struct mstdnt_fetch_results* results,
                                     size_t* size)
{
    size_t i = 0;
    cJSON* root, *status_j_list;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    if (!cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *statuses = malloc((size ? *size : cJSON_GetArraySize(root))
                       * sizeof(struct mstdnt_status));
    if (*statuses == NULL)
        return 1;
    
    cJSON_ArrayForEach(status_j_list, root)
    {
        mstdnt_load_status_from_json((*statuses) + i++, status_j_list->child);
    }
    storage->needs_cleanup = 1;
    return 0;
}

int mastodont_account_statuses(mastodont_t* data,
                               char* id,
                               struct mstdnt_args* args,
                               struct mstdnt_storage* storage,
                               struct mstdnt_status* statuses[],
                               size_t* size)
{
    int res;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/statuses", id);
    
    /* Default args */
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_HTTPGET) != CURLE_OK)
        return 1;

    res = mstdnt_load_statuses_from_result(statuses, storage, &results, size);

    mastodont_fetch_results_cleanup(&results);
    
    return res;
}

int mastodont_create_status(mastodont_t* data,
                            struct mstdnt_args* args,
                            struct mstdnt_storage* storage)
{
    int res = 0;
    char* post;
    struct mstdnt_fetch_results results = { 0 };

    /* Default args */
    storage->needs_cleanup = 0;

    union param_value u_content_type, u_expires_in,
        u_in_reply_to_conversation_id, u_in_reply_to_id,
        u_language, u_media_ids, u_poll, u_preview, u_scheduled_at,
        u_sensitive, u_spoiler_text, u_status, u_visibility;
    u_content_type.s = args->content_type;
    u_expires_in.i = args->expires_in;
    u_in_reply_to_conversation_id.s = args->in_reply_to_conversation_id;
    u_in_reply_to_id.s = args->in_reply_to_id;
    u_language.s = args->language;
    /*u_media_ids.s = args->media_ids;*/
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
    };

    post = _mstdnt_query_string(data, NULL, params, _mstdnt_arr_len(params));

    curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);
            
    if (mastodont_fetch_curl(data, "api/v1/statuses", &results, CURLOPT_POST) != CURLE_OK)
        return 1;

    mastodont_fetch_results_cleanup(&results);
    return 0;
}

int mastodont_favourite_status(mastodont_t* data,
                               char* id,
                               struct mstdnt_storage* storage)
{
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE,
             "api/v1/statuses/%s/favourite", id);

    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_POST) != CURLE_OK)
        return 1;

    mastodont_fetch_results_cleanup(&results);
    return 0;
}


int mastodont_reblog_status(mastodont_t* data,
                            char* id,
                            struct mstdnt_storage* storage)
{
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE,
             "api/v1/statuses/%s/reblog", id);

    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_POST) != CURLE_OK)
        return 1;

    mastodont_fetch_results_cleanup(&results);
    return 0;
}

int mastodont_view_status(mastodont_t* data,
                          char* id,
                          struct mstdnt_storage* storage,
                          struct mstdnt_status* status)
{
    int res;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s", id);
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_HTTPGET) != CURLE_OK)
        return 1;

    res = mstdnt_load_status_from_result(status, storage, &results);

    mastodont_fetch_results_cleanup(&results);
    
    return res;    
}

int mastodont_status_context(mastodont_t* data,
                             char* id,
                             struct mstdnt_storage* storage,
                             struct mstdnt_status* statuses_before[],
                             struct mstdnt_status* statuses_after[],
                             size_t* size_before,
                             size_t* size_after)
{
    int res = 0;
    cJSON* root, *v, *status_item;
    size_t* size_ptr;
    struct mstdnt_status** stat_ptr = NULL;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/statuses/%s/context", id);
    storage->needs_cleanup = 0;

    *size_before = 0;
    *size_after = 0;
    *statuses_before = NULL;
    *statuses_after = NULL;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_HTTPGET) != CURLE_OK)
    {
        return 1;
    }

    if (_mstdnt_json_init(&root, &results, storage))
    {
        res = 1;
        goto cleanup;
    }
    storage->root = root;

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

            *stat_ptr = malloc(cJSON_GetArraySize(v) * sizeof(struct mstdnt_status));
            if (*stat_ptr == NULL)
                return 1;
            
            cJSON_ArrayForEach(status_item, v)
                mstdnt_load_status_from_json((*stat_ptr) + (*size_ptr)++, status_item->child);
        }
    }
    storage->needs_cleanup = 1;
cleanup:
    mastodont_fetch_results_cleanup(&results);

    return res;
}

void cleanup_status(struct mstdnt_status* status)
{
    cleanup_attachments(status->media_attachments);
    cleanup_status_pleroma(&(status->pleroma));
}

void cleanup_statuses(struct mstdnt_status* statuses, size_t s)
{
    size_t i;
    if (!statuses) return;
    for (i = 0; i < s; ++i)
    {
        cleanup_status(statuses + i);
    }
    free(statuses);
}
