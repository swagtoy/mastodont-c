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
#include <cjson/cJSON.h>

struct mstdnt_notification
{
    char* id;
    char* created_at;
    struct mstdnt_account* account;
    struct mstdnt_status* status;
    enum mstdnt_notification_type type;
};

int mastodont_get_notifications(mastodont_t* data,
                                struct mstdnt_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_notification** notifs,
                                size_t* size);

int mstdnt_notification_from_json(struct mstdnt_notification* notif, cJSON* js);
void mstdnt_cleanup_notifications(struct mstdnt_notification* notif, size_t notif_len);
void mstdnt_cleanup_notification(struct mstdnt_notification* notif);

#endif /* MASTODONT_NOTIFICATION */
