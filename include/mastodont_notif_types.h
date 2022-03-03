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

enum mstdnt_notification_type
{
    MSTDNT_NOTIFICATION_FOLLOW,
    MSTDNT_NOTIFICATION_FOLLOW_REQUEST,
    MSTDNT_NOTIFICATION_MENTION,
    MSTDNT_NOTIFICATION_REBLOG,
    MSTDNT_NOTIFICATION_FAVOURITE,
    MSTDNT_NOTIFICATION_POLL,
    MSTDNT_NOTIFICATION_STATUS,
    MSTDNT_NOTIFICATION_EMOJI_REACT,
    MSTDNT_NOTIFICATION_CHAT_MENTION,
    MSTDNT_NOTIFICATION_REPORT
};

#endif /* MASTODONT_NOTIF_TYPES */
