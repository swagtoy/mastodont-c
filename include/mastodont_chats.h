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
#include <stddef.h>
#include <time.h>
#include "mastodont_types.h"
#include "mastodont_account.h"
#include "mastodont_emoji.h"

struct mstdnt_chat
{
    struct mstdnt_account account;
    const char* id;
    unsigned int unread;
};

struct mstdnt_message
{
    char* account_id;
    char* chat_id;
    char* content;
    time_t created_at;
    char* id;
    struct mstdnt_emoji* emojis;
    size_t emojis_len;
    mstdnt_bool unread;
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

int mstdnt_chat_json(struct mstdnt_chat* chat, cJSON* js);
int mstdnt_chats_json(struct mstdnt_chat* statuses[],
                      size_t* size,
                      cJSON* js);
int mstdnt_message_json(struct mstdnt_message* message, cJSON* js);
int mstdnt_message_json_callback(cJSON* json, void* chat);
int mstdnt_messages_json(struct mstdnt_message* message[], size_t* size, cJSON* js);

int mastodont_get_chats_v2(mastodont_t* data,
                           struct mstdnt_args* m_args,
                           struct mstdnt_chats_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_chat* chats[],
                           size_t* chats_len);

int mastodont_get_chat_messages(mastodont_t* data,
                                struct mstdnt_args* m_args,
                                char* chat_id,
                                struct mstdnt_chats_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_message* chats[],
                                size_t* size);

int mastodont_get_chat(mastodont_t* data,
                       struct mstdnt_args* m_args,
                       char* chat_id,
                       struct mstdnt_storage* storage,
                       struct mstdnt_chat* chat);

void mstdnt_cleanup_chat(struct mstdnt_chat* chat);
void mstdnt_cleanup_chats(struct mstdnt_chat* chats, size_t len);
void mstdnt_cleanup_messages(struct mstdnt_message* chats);

#endif // MASTODONT_CHATS_H
