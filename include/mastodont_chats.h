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

#ifndef MASTODONT_CHATS_H
#define MASTODONT_CHATS_H
#include "mastodont_types.h"
#include "mastodont_account.h"

struct mstdnt_chat
{
    struct mstdnt_account account;
    const char* id;
    int unread;
};

struct mstdnt_chats_args
{
    mstdnt_bool with_muted;
    const char* max_id;
    const char* min_id;
    const char* since_id;
    int offset;
    int limit;    
};

int mastodont_get_chats_v2(mastodont_t* data,
                           struct mstdnt_args* m_args,
                           struct mstdnt_chats_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_chat* chats[],
                           size_t* chats_len);

#endif // MASTODONT_CHATS_H
