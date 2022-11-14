/*
 * Licensed under BSD 2-Clause License
 */

#include <stdlib.h>
#include <string.h>
#include <mastodont_hooks.h>
#include <mastodont_notification.h>
#include <mastodont_fetch.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

static void
_mstdnt_val_notif_type_call(cJSON* v, void* _type)
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

static void
_mstdnt_val_malloc_notification_pleroma_call(cJSON* v, void* _type)
{
    struct mstdnt_notification_pleroma** type = _type;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_notification_pleroma));

    if (*type)
        mstdnt_notification_pleroma_json(*type, v->child);
}

int
mstdnt_notification_json(struct mstdnt_notification* notif, cJSON* js)
{
    cJSON* v;

    if (!notif) return 1;

    /* Zero out */
    memset(notif, 0, sizeof(struct mstdnt_notification));

    struct _mstdnt_val_ref vals[] = {
        { "account", &(notif->account), _mstdnt_val_malloc_account_call },
        { "created_at", &(notif->created_at), _mstdnt_val_datetime_unix_call },
        { "emoji", &(notif->emoji), _mstdnt_val_string_call },
        { "id", &(notif->id), _mstdnt_val_string_call },
        { "status", &(notif->status), _mstdnt_val_malloc_status_call },
        { "pleroma", &(notif->pleroma), _mstdnt_val_malloc_notification_pleroma_call },
        { "type", &(notif->type), _mstdnt_val_notif_type_call },
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

int
mstdnt_notification_pleroma_json(struct mstdnt_notification_pleroma* notif, cJSON* js)
{
    cJSON* v;

    if (!notif) return 1;

    /* Zero out */
    memset(notif, 0, sizeof(struct mstdnt_notification_pleroma));

    struct _mstdnt_val_ref vals[] = {
        { "is_muted", &(notif->is_muted), _mstdnt_val_bool_call },
        { "is_seen", &(notif->is_seen), _mstdnt_val_bool_call },
    };
    
    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));
    
    return 0;
}

int
mstdnt_notification_json_callback(cJSON* json,
                                  void* args,
                                  mstdnt_request_cb_data* data)
{
    (void)args;
    struct mstdnt_notification* notif = malloc(sizeof(struct mstdnt_notification));
    data->data = notif;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_notification;
    return mstdnt_notification_json(notif, json->child);
}

GENERATE_JSON_ARRAY_FUNC(mstdnt_notifications_json, struct mstdnt_notification, mstdnt_notification_json)

int
mstdnt_notifications_json_callback(cJSON* json,
                                   void* args,
                                   mstdnt_request_cb_data* data)
{
    (void)args;
    struct mstdnt_notifications* notifs = malloc(sizeof(struct mstdnt_notifications));
    data->data = notifs;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_notifications;
    return mstdnt_notifications_json(&(notifs->notifs), &(notifs->len), json);
}

int
mstdnt_get_notifications(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         struct mstdnt_notifications_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "account_id", { .s = args.account_id } },
        { _MSTDNT_QUERY_INT, "with_muted", { .i = args.with_muted } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/notifications",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_notifications_json_callback,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_notification_dismiss(mastodont_t* data,
                            struct mstdnt_args* m_args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            char* id)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/notifications/%s/dismiss", id);
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .request_type = CURLOPT_POST,
        // TODO?
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);   
}

int
mstdnt_notifications_clear(mastodont_t* data,
                           struct mstdnt_args* m_args,
                           mstdnt_request_cb_t cb_request,
                           void* cb_args)
{
    struct mstdnt_request_args req_args = {
        .url = "api/v1/notifications/clear",
        .request_type = CURLOPT_POST,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_notifications_read(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          mstdnt_request_cb_t cb_request,
                          void* cb_args,
                          struct mstdnt_notifications_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "id", { .s = args.id } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/pleroma/notifications/read",
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_POST,
        .callback = mstdnt_notification_json_callback,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void
mstdnt_cleanup_notifications(struct mstdnt_notifications* notifs)
{
    size_t i;
    if (!notifs) return;
    for (i = 0; i < notifs->len; ++i)
        mstdnt_cleanup_notification(notifs->notifs + i);

    mstdnt_free(notifs->notifs);
}

void
mstdnt_cleanup_notification(struct mstdnt_notification* notif)
{
    if (notif->account)
    {
        mstdnt_cleanup_account(notif->account);
        mstdnt_free(notif->account);
    }
    if (notif->status)
    {
        mstdnt_cleanup_status(notif->status);
        mstdnt_free(notif->status);
    }
    mstdnt_free(notif->pleroma);
}

const char*
mstdnt_notification_t_to_str(mstdnt_notification_t type)
{
    switch (type)
    {
    case MSTDNT_NOTIFICATION_FOLLOW: return "follow";
    case MSTDNT_NOTIFICATION_FOLLOW_REQUEST: return "follow request";
    case MSTDNT_NOTIFICATION_MENTION: return "mention";
    case MSTDNT_NOTIFICATION_REBLOG: return "reblog";
    case MSTDNT_NOTIFICATION_FAVOURITE: return "favourite";
    case MSTDNT_NOTIFICATION_POLL: return "poll";
    case MSTDNT_NOTIFICATION_STATUS: return "status";
    case MSTDNT_NOTIFICATION_EMOJI_REACT: return "emoji reaction";
    case MSTDNT_NOTIFICATION_CHAT_MENTION: return "chat mention";
    case MSTDNT_NOTIFICATION_REPORT: return "report";
    case MSTDNT_NOTIFICATION_NOOP:
    default:
        return "unknown";
    }
}
