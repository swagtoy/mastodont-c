/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_CHATS_H
#define MASTODONT_CHATS_H
#include <stddef.h>
#include <time.h>
#include "mastodont_types.h"
#include "mastodont_account.h"
#include "mastodont_emoji.h"

#ifdef __cplusplus
extern "C" {
#endif

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

struct mstdnt_chats
{
    struct mstdnt_chat* chats;
    size_t len;
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

int
mstdnt_chat_json(struct mstdnt_chat* chat, cJSON* js);

int
mstdnt_chats_json(struct mstdnt_chat* statuses[],
                  size_t* size,
                  cJSON* js);
int
mstdnt_message_json(struct mstdnt_message* message, cJSON* js);

int
mstdnt_message_json_callback(cJSON* json,
                             void* chat,
                             mstdnt_request_cb_data* data);

int
mstdnt_chat_json_callback(cJSON* json,
                          void* args,
                          mstdnt_request_cb_data* data);

int mstdnt_messages_json(struct mstdnt_message* message[],
                         size_t* size,
                         cJSON* js);

int
mstdnt_get_chats_v2(mastodont_t* data,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    struct mstdnt_chats_args args);

int
mstdnt_get_chat_messages(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* chat_id,
                         struct mstdnt_chats_args args);

int
mstdnt_get_chat(mastodont_t* data,
                struct mstdnt_args* m_args,
                mstdnt_request_cb_t cb_request,
                void* cb_args,
                char* chat_id);

void mstdnt_cleanup_chat(struct mstdnt_chat* chat);
void mstdnt_cleanup_chats(struct mstdnt_chats* chats);
void mstdnt_cleanup_message(struct mstdnt_message* chats);

#ifdef __cplusplus
}
#endif

#endif // MASTODONT_CHATS_H
