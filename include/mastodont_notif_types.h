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

#ifndef MASTODONT_NOTIF_TYPES
#define MASTODONT_NOTIF_TYPES
#include <stdint.h>

/* uint16 */
#define MSTDNT_NOTIFICATION_NOOP 0
#define MSTDNT_NOTIFICATION_FOLLOW (1<<0)
#define MSTDNT_NOTIFICATION_FOLLOW_REQUEST (1<<1)
#define MSTDNT_NOTIFICATION_MENTION (1<<2)
#define MSTDNT_NOTIFICATION_REBLOG (1<<3)
#define MSTDNT_NOTIFICATION_FAVOURITE (1<<4)
#define MSTDNT_NOTIFICATION_POLL (1<<5)
#define MSTDNT_NOTIFICATION_STATUS (1<<6)
#define MSTDNT_NOTIFICATION_EMOJI_REACT (1<<7)
#define MSTDNT_NOTIFICATION_CHAT_MENTION (1<<8)
#define MSTDNT_NOTIFICATION_REPORT (1<<9)

typedef uint16_t mstdnt_notification_t;

/**
 * Turns a notification type into a string
 *
 * @param type Notification type.
 * @return String literal representing type, e.g. 'follow', 'follow request'
 */
const char* mstdnt_notification_t_to_str(mstdnt_notification_t type);

#endif /* MASTODONT_NOTIF_TYPES */
