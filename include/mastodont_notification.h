/*
 * Licensed under BSD 2-Clause License
 */

#ifndef MASTODONT_NOTIFICATION
#define MASTODONT_NOTIFICATION
#include <time.h>
#include "mastodont_types.h"
#include "mastodont_account.h"
#include "mastodont_status.h"
#include "mastodont_notif_types.h"
#include "mastodont_visibility_types.h"
#include <cjson/cJSON.h>

struct mstdnt_notification_pleroma
{
    unsigned is_muted;
    unsigned is_seen;
};

struct mstdnt_notification
{
    char* id;
    time_t created_at;
    char* emoji;
    struct mstdnt_account* account;
    struct mstdnt_status* status;
    struct mstdnt_notification_pleroma* pleroma;
    mstdnt_notification_t type;
};

struct mstdnt_notifications
{
    struct mstdnt_notification* notifs;
    size_t len;
};

struct mstdnt_notifications_args
{
    /* Bitmask */
    mstdnt_notification_t exclude_types;
    char* account_id;
    /* Bitmask */
    mstdnt_visibility_t* exclude_visibilities;
    /* Bitmask */
    mstdnt_visibility_t* include_types;
    mstdnt_bool with_muted;
    char* max_id;
    char* min_id;
    char* id;
    char* since_id;
    int offset;
    int limit;
};

struct _mstdnt_notifications_result_cb_args
{
    struct mstdnt_notification** notif;
    size_t* size;
};

// Callback helpers
int
mstdnt_notification_json_callback(cJSON* json,
                                  void* args,
                                  mstdnt_request_cb_data* data);

int
mstdnt_notifications_json_callback(cJSON* json,
                                   void* args,
                                   mstdnt_request_cb_data* data);

int
mstdnt_notification_json(struct mstdnt_notification* notif, cJSON* js);

int
mstdnt_notification_pleroma_json(struct mstdnt_notification_pleroma* notif, cJSON* js);

int
mstdnt_get_notifications(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         struct mstdnt_notifications_args args);

int
mstdnt_notifications_clear(mastodont_t* data,
                           struct mstdnt_args* m_args,
                           mstdnt_request_cb_t cb_request,
                           void* cb_args);

int
mstdnt_notifications_read(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          mstdnt_request_cb_t cb_request,
                          void* cb_args,
                          struct mstdnt_notifications_args args);

int
mstdnt_notification_dismiss(mastodont_t* data,
                            struct mstdnt_args* m_args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            char* id);

void mstdnt_cleanup_notifications(struct mstdnt_notifications* notif);
void mstdnt_cleanup_notification(struct mstdnt_notification* notif);

#endif /* MASTODONT_NOTIFICATION */
