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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mastodont_account.h>
#include <mastodont_request.h>
#include <mastodont_json_helper.h>
#include <mastodont_generate.h>

void _mstdnt_val_account_call(cJSON* v, void* _type)
{
    struct mstdnt_account* type = _type;

    mstdnt_account_json(type, v->child);
}

void _mstdnt_val_malloc_account_call(cJSON* v, void* _type)
{
    struct mstdnt_account** type = _type;

    *type = calloc(1, sizeof(struct mstdnt_account));

    if (*type)
        mstdnt_account_json(*type, v->child);
}

// GENERATE mstdnt_statuses_json
GENERATE_JSON_ARRAY_FUNC(mstdnt_accounts_json, struct mstdnt_account, mstdnt_account_json)

int mstdnt_account_json_callback(cJSON* json, void* _args)
{
    // Not sure why it expects it to be in the child
    return mstdnt_account_json(_args, json->child);
}

int mstdnt_accounts_json_callback(cJSON* json, void* _args)
{
    struct _mstdnt_accounts_args* args = _args;
    return mstdnt_accounts_json(args->acct, args->size, json);
}

static int mastodont_get_accounts_query(char* url,
                                        mastodont_t* data,
                                        struct mstdnt_args* m_args,
                                        struct mstdnt_account_args* args,
                                        struct mstdnt_storage* storage,
                                        struct mstdnt_account* accts[],
                                        size_t* accts_len)
{
    struct _mstdnt_accounts_args _args = {
        accts,
        accts_len,
    };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "with_relationships", { .i = args->with_relationships } },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        &_args,
        mstdnt_accounts_json_callback,
    };

    return mastodont_request(data, m_args, &req_args);
}

int mastodont_get_followers(mastodont_t* data,
                            struct mstdnt_args* m_args,
                            char* id,
                            struct mstdnt_account_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_account* accts[],
                            size_t* accts_len)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/followers", id);
    return mastodont_get_accounts_query(url, data, m_args, args, storage, accts, accts_len);
}

int mastodont_get_following(mastodont_t* data,
                            struct mstdnt_args* m_args,
                            char* id,
                            struct mstdnt_account_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_account* accts[],
                            size_t* accts_len)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/following", id);
    return mastodont_get_accounts_query(url, data, m_args, args, storage, accts, accts_len);
}

int mastodont_get_blocks(mastodont_t* data, struct mstdnt_args* m_args,
                         struct mstdnt_account_args* args,
                         struct mstdnt_storage* storage,
                         struct mstdnt_account* accts[],
                         size_t* accts_len)
{
    return mastodont_get_accounts_query("api/v1/blocks", data, m_args, args, storage, accts, accts_len);
}

int mastodont_get_mutes(mastodont_t* data, struct mstdnt_args* m_args,
                        struct mstdnt_account_args* args,
                        struct mstdnt_storage* storage,
                        struct mstdnt_account* accts[],
                        size_t* accts_len)
{
    return mastodont_get_accounts_query("api/v1/mutes", data, m_args, args, storage, accts, accts_len);
}

int mastodont_get_account(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          int lookup, /* TODO move into separate function for consistancy? */
                          char* id,
                          struct mstdnt_account* acct,
                          struct mstdnt_storage* storage)
{
    /* Url */
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE,
             lookup ? "api/v1/accounts/%s" : "api/v1/accounts/lookup?acct=%s",
             id);
    
    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        acct, /* args */
        mstdnt_account_json_callback, /* callback */
    };
    
    return mastodont_request(data, m_args, &req_args);
}

int mastodont_verify_credentials(mastodont_t* data,
                                 struct mstdnt_args* m_args,
                                 struct mstdnt_account* acct,
                                 struct mstdnt_storage* storage)
{
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/accounts/verify_credentials",
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        acct, /* args */
        mstdnt_account_json_callback, /* callback */
    };
    
    return mastodont_request(data, m_args, &req_args);
}

int mstdnt_account_json(struct mstdnt_account* acct, cJSON* js)
{
    cJSON* v;
    
    /* Zero out */
    memset(acct, 0, sizeof(struct mstdnt_account));

    struct _mstdnt_generic_args emj_args = {
        &(acct->emojis),
        &(acct->emojis_len)
    };

    struct _mstdnt_val_ref refs[] = {
        { "id", &(acct->id), _mstdnt_val_string_call },
        { "username", &(acct->username), _mstdnt_val_string_call },
        { "acct", &(acct->acct), _mstdnt_val_string_call },
        { "display_name", &(acct->display_name), _mstdnt_val_string_call },
        { "created_at", &(acct->created_at), _mstdnt_val_datetime_unix_call },
        { "note", &(acct->note), _mstdnt_val_string_call },
        { "url", &(acct->url), _mstdnt_val_string_call },
        { "avatar", &(acct->avatar), _mstdnt_val_string_call },
        { "avatar_static", &(acct->avatar_static), _mstdnt_val_string_call },
        { "emojis", &emj_args, _mstdnt_val_emojis_call },
        { "header", &(acct->header), _mstdnt_val_string_call },
        { "header_static", &(acct->header_static), _mstdnt_val_string_call },
        { "last_status_at", &(acct->last_status_at), _mstdnt_val_string_call },
        { "mute_expires_at", &(acct->mute_expires_at), _mstdnt_val_string_call },
        { "locked", &(acct->locked), _mstdnt_val_bool_call },
        { "bot", &(acct->bot), _mstdnt_val_bool_call },
        { "statuses_count", &(acct->statuses_count), _mstdnt_val_uint_call },
        { "followers_count", &(acct->followers_count), _mstdnt_val_uint_call },
        { "following_count", &(acct->following_count), _mstdnt_val_uint_call },
    };
    
    for (v = js; v; v = v->next)
    {
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    }
    
    return 0;
}


int mstdnt_account_action(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          char* id,
                          struct mstdnt_storage* storage,
                          struct mstdnt_relationship* rel,
                          char* url_str)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, url_str, id);

    struct mastodont_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_POST,
        NULL,
        rel,
        mstdnt_relationship_json_callback
    };

    return mastodont_request(data, m_args, &req_args);
}

/* These are all the same */
MSTDNT_ACCOUNT_ACTION_DECL(follow)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("follow")
    
MSTDNT_ACCOUNT_ACTION_DECL(unfollow)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("unfollow")
    
MSTDNT_ACCOUNT_ACTION_DECL(mute)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("mute")
    
MSTDNT_ACCOUNT_ACTION_DECL(unmute)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("unmute")
    
MSTDNT_ACCOUNT_ACTION_DECL(block)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("block")
    
MSTDNT_ACCOUNT_ACTION_DECL(unblock)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("unblock")
    
MSTDNT_ACCOUNT_ACTION_DECL(subscribe)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("subscribe")
    
MSTDNT_ACCOUNT_ACTION_DECL(unsubscribe)
MSTDNT_ACCOUNT_ACTION_FUNC_URL("unsubscribe")

void mstdnt_cleanup_account(struct mstdnt_account* acct)
{
    if (!acct) return;
    mstdnt_cleanup_emojis(acct->emojis);
}

void mstdnt_cleanup_accounts(struct mstdnt_account* accts, size_t len)
{
    if (!accts) return;
    for (int i = 0; i < len; ++i)
        mstdnt_cleanup_account(accts + i);
    free(accts);
}

