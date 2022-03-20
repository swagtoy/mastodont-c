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
#include <string.h>
#include <mastodont_notification.h>
#include <mastodont_fetch.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

static void _mstdnt_val_notif_type_call(cJSON* v, void* _type)
{
    mstdnt_notification_t* type = _type;

    if (strcmp(v->string, "type") != 0)
    {
        *type = 0;
        return;
    }

    /* Sorted by most common notification type :^) */
    if (strcmp(v->valuestring, "favourite") == 0) *type = MSTDNT_NOTIFICATION_FAVOURITE;
    else if (strcmp(v->valuestring, "mention") == 0) *type = MSTDNT_NOTIFICATION_MENTION;
    else if (strcmp(v->valuestring, "reblog") == 0) *type = MSTDNT_NOTIFICATION_REBLOG;
    else if (strcmp(v->valuestring, "pleroma:emoji_reaction") == 0) *type = MSTDNT_NOTIFICATION_EMOJI_REACT;
    else if (strcmp(v->valuestring, "follow") == 0) *type = MSTDNT_NOTIFICATION_FOLLOW;
    else if (strcmp(v->valuestring, "follow_request") == 0) *type = MSTDNT_NOTIFICATION_FOLLOW;
    else if (strcmp(v->valuestring, "poll") == 0) *type = MSTDNT_NOTIFICATION_POLL;
    else if (strcmp(v->valuestring, "status") == 0) *type = MSTDNT_NOTIFICATION_STATUS;
    else if (strcmp(v->valuestring, "pleroma:chat_mention") == 0) *type = MSTDNT_NOTIFICATION_CHAT_MENTION;
    else if (strcmp(v->valuestring, "pleroma:report") == 0) *type = MSTDNT_NOTIFICATION_REPORT;
}

int mstdnt_notification_from_json(struct mstdnt_notification* notif, cJSON* js)
{
    cJSON* v;

    struct _mstdnt_val_ref vals[] = {
        { "account", &(notif->account), _mstdnt_val_malloc_account_call },
        { "created_at", &(notif->created_at), _mstdnt_val_string_call },
        { "id", &(notif->id), _mstdnt_val_string_call },
        { "status", &(notif->status), _mstdnt_val_malloc_status_call },
        /* { "pleroma", &(notif->pleroma), _mstdnt_val_notif_pleroma_call }, */
        { "type", &(notif->type), _mstdnt_val_notif_type_call },
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

int mstdnt_notifications_from_result(struct mstdnt_fetch_results* results,
                                     struct mstdnt_storage* storage,
                                     struct mstdnt_notification* notif[],
                                     size_t* size)
{
    size_t i = 0;
    cJSON* root, *notif_j_list;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *notif = malloc((size ? *size : cJSON_GetArraySize(root))
                    * sizeof(struct mstdnt_notification));
    if (*notif == NULL)
        return 1;
    
    cJSON_ArrayForEach(notif_j_list, root)
    {
        /* Null out values incase no match */
        (*notif)[i].account = NULL;
        (*notif)[i].status = NULL;
        /* notif[i]->pleroma = NULL; */
        
        mstdnt_notification_from_json((*notif) + i++, notif_j_list->child);
    }

    return 0;
}

int _mstdnt_notifications_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args)
{
    struct _mstdnt_notifications_result_cb_args* args = _args;
    return mstdnt_notifications_from_result(results, storage, args->notif, args->size);
}


int mastodont_get_notifications(mastodont_t* data,
                                struct mstdnt_get_notifications_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_notification** notifs,
                                size_t* size)
{
    struct _mstdnt_notifications_result_cb_args cb_args = { notifs, size };
    
    union param_value u_exclude_types, u_account_id, u_exclude_visibilities,
        u_include_types, u_with_muted, u_max_id, u_min_id,
        u_since_id, u_offset, u_limit;
    /* TODO Arrays of excludes, includes */
    u_account_id.s = args->account_id;
    u_with_muted.i = args->with_muted;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_offset.i = args->offset;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "account_id", u_account_id },
        { _MSTDNT_QUERY_INT, "with_muted", u_with_muted },
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/notifications",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_notifications_result_callback,
    };
    
    return mastodont_request(data, &req_args);
}

void mstdnt_cleanup_notifications(struct mstdnt_notification* notifs, size_t notifs_len)
{
    size_t i;
    if (!notifs) return;
    for (i = 0; i < notifs_len; ++i)
        mstdnt_cleanup_notification(notifs + i);

    free(notifs);
}

void mstdnt_cleanup_notification(struct mstdnt_notification* notif)
{
    if (notif->account) free(notif->account);
    if (notif->status)
    {
        mstdnt_cleanup_status(notif->status);
        free(notif->status);
    }
}

