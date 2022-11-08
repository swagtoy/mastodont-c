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

#include <string.h>
#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_chats.h>
#include <mastodont_fetch.h>
#include <mastodont_query.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>
#include <mastodont_json_helper.h>
#include <mastodont_emoji.h>

struct _mstdnt_chats_cb_args
{
    struct mstdnt_chat** chats;
    size_t* chats_len;
};

struct _mstdnt_messages_cb_args
{
    struct mstdnt_message** messages;
    size_t* messages_len;
};

int
mstdnt_chat_json(struct mstdnt_chat* chat, cJSON* js)
{
    memset(chat, 0, sizeof(struct mstdnt_chat));

    struct _mstdnt_val_ref vals[] = {
        { "account", &(chat->account), _mstdnt_val_account_call },
        { "id", &(chat->id), _mstdnt_val_string_call },
        { "unread", &(chat->unread), _mstdnt_val_uint_call },
    };

    for (cJSON* v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));

    return 0;
}

int
mstdnt_message_json(struct mstdnt_message* message, cJSON* js)
{
    memset(message, 0, sizeof(struct mstdnt_message));

    struct _mstdnt_generic_args emj_args = {
        &(message->emojis),
        &(message->emojis_len)
    };

    struct _mstdnt_val_ref vals[] = {
        { "account_id", &(message->account_id), _mstdnt_val_string_call },
        { "chat_id", &(message->chat_id), _mstdnt_val_string_call },
        { "content", &(message->content), _mstdnt_val_string_call },
        { "created_at", &(message->created_at), _mstdnt_val_datetime_unix_call },
        { "id", &(message->id), _mstdnt_val_string_call },
        { "emojis", &emj_args, _mstdnt_val_emojis_call },
        { "unread", &(message->unread), _mstdnt_val_uint_call },
    };

    for (cJSON* v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));

    return 0;
}

int
mstdnt_message_json_callback(cJSON* json,
                             void* args,
                             mstdnt_request_cb_data* data)
{
    struct mstdnt_message* msg = malloc(sizeof(struct mstdnt_message));
    data->data = msg;
    data->data_free_cb = (mstdnt_request_cb_t)mstdnt_cleanup_message;
    return mstdnt_message_json(msg, json->child);
}

int
mstdnt_chat_json_callback(cJSON* json,
                          void* args,
                          mstdnt_request_cb_data* data)
{
    struct mstdnt_chat* chat = malloc(sizeof(struct mstdnt_chat));
    data->data = chat;
    data->data_free_cb = (mstdnt_request_cb_t)mstdnt_cleanup_chats;
    return mstdnt_chat_json(chat, json->child);
}

// GENERATE mstdnt_chats_json
GENERATE_JSON_ARRAY_FUNC(mstdnt_chats_json, struct mstdnt_chat, mstdnt_chat_json)

// GENERATE mstdnt_messages_json
GENERATE_JSON_ARRAY_FUNC(mstdnt_messages_json, struct mstdnt_message, mstdnt_message_json)

static int
mstdnt_chats_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_chats_cb_args* args = _args;
    return mstdnt_chats_json(args->chats, args->chats_len, json);
}

static int
mstdnt_messages_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_messages_cb_args* args = _args;
    return mstdnt_messages_json(args->messages, args->messages_len, json);
}

int
mstdnt_get_chats_v2(mastodont_t* data,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    struct mstdnt_chats_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
    };

    struct mstdnt_request_args req_args = {
        .url = "api/v2/pleroma/chats",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_chats_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_get_chat_messages(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* chat_id,
                         struct mstdnt_chats_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/chats/%s/messages", chat_id);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
    };

    struct mstdnt_request_args req_args = {
        .url = url,
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_messages_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_get_chat(mastodont_t* data,
                struct mstdnt_args* m_args,
                mstdnt_request_cb_t cb_request,
                void* cb_args,
                char* chat_id)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/pleroma/chats/%s", chat_id);
    
    struct mstdnt_request_args req_args = {
        .url = url,
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_chat_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void mstdnt_cleanup_chat(struct mstdnt_chat* chat)
{
    mstdnt_cleanup_account(&(chat->account));
}


void mstdnt_cleanup_chats(struct mstdnt_chats* chats)
{
    if (!chats) return;
    for (size_t i = 0; i < chats->len; ++i)
        mstdnt_cleanup_chat(chats->chats + i);
    mstdnt_free(chats->chats);
}

void mstdnt_cleanup_message(struct mstdnt_message* messages)
{
    if (!messages) return;
    mstdnt_cleanup_emojis(messages->emojis);
}
