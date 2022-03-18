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

#ifndef MASTODONT_NOTIFICATION
#define MASTODONT_NOTIFICATION
#include "mastodont_types.h"
#include "mastodont_account.h"
#include "mastodont_status.h"
#include "mastodont_args.h"
#include "mastodont_notif_types.h"
#include "mastodont_visibility_types.h"
#include <cjson/cJSON.h>

struct mstdnt_notification
{
    char* id;
    char* created_at;
    struct mstdnt_account* account;
    struct mstdnt_status* status;
    mstdnt_notification_t type;
};

struct mstdnt_get_notifications_args
{
    /* Bitmask */
    mstdnt_notification_t exclude_types;
    char* account_id;
    /* Bitmask */
    mstdnt_visibility_t exclude_visibilities;
    /* Bitmask */
    mstdnt_visibility_t include_types;
    mstdnt_bool with_muted;
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
};

struct _mstdnt_notifications_result_cb_args
{
    struct mstdnt_notification** notif;
    size_t* size;
};

int _mstdnt_notifications_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args);

int mastodont_get_notifications(mastodont_t* data,
                                struct mstdnt_get_notifications_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_notification** notifs,
                                size_t* size);

int mstdnt_notification_from_json(struct mstdnt_notification* notif, cJSON* js);
void mstdnt_cleanup_notifications(struct mstdnt_notification* notif, size_t notif_len);
void mstdnt_cleanup_notification(struct mstdnt_notification* notif);

#endif /* MASTODONT_NOTIFICATION */
