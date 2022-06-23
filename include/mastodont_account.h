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

#ifndef MASTODONT_ACCOUNT
#define MASTODONT_ACCOUNT
#include "mastodont_types.h"
#include "mastodont_fetch.h"
#include "mastodont_relationship.h"
#include "mastodont_emoji.h"
#include <cjson/cJSON.h>

#define MSTDNT_LOOKUP_ACCT 0
#define MSTDNT_LOOKUP_ID 1

struct _mstdnt_accounts_args
{
    struct mstdnt_account** acct;
    size_t* size;
};

struct mstdnt_account
{
    char* id;
    char* username;
    char* acct;
    char* url;

    /* Display attributes */
    char* display_name;
    char* note;
    char* avatar;
    char* avatar_static;
    char* header;
    char* header_static;
    mstdnt_bool locked;
    mstdnt_bool discoverable;

    struct mstdnt_emoji* emojis;
    size_t emojis_len;

    /* Statistic attributes */
    char* created_at;
    char* last_status_at;
    unsigned statuses_count;
    unsigned followers_count;
    unsigned following_count;

    /* Optional attributes */
    struct mstdnt_account* moved;
    /* struct mstdnt_field* field; */
    mstdnt_bool bot;
    /* struct mstdnt_source* source */
    mstdnt_bool suspended;
    char* mute_expires_at;
};

struct mstdnt_account_args
{
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
    int with_relationships; // mastodont_get_mutes
};

int mstdnt_account_action(mastodont_t* data, struct mstdnt_args* args,
                          char* id,
                          struct mstdnt_storage* storage,
                          struct mstdnt_relationship* rel,
                          char* url_str);

int mstdnt_accounts_json(struct mstdnt_account* accounts[],
                         size_t* size,
                         cJSON* js);

#define MSTDNT_ACCOUNT_ACTION_DECL(type) int mastodont_##type##_account(mastodont_t* data, struct mstdnt_args* args, char* id, struct mstdnt_storage* storage, struct mstdnt_relationship* relationship)
#define MSTDNT_ACCOUNT_ACTION_FUNC_URL(action) { \
    return mstdnt_account_action(data, id, storage, relationship, "api/v1/accounts/%s/" action); \
    }

MSTDNT_ACCOUNT_ACTION_DECL(follow);
MSTDNT_ACCOUNT_ACTION_DECL(unfollow);
MSTDNT_ACCOUNT_ACTION_DECL(mute);
MSTDNT_ACCOUNT_ACTION_DECL(unmute);
MSTDNT_ACCOUNT_ACTION_DECL(block);
MSTDNT_ACCOUNT_ACTION_DECL(unblock);
MSTDNT_ACCOUNT_ACTION_DECL(subscribe);
MSTDNT_ACCOUNT_ACTION_DECL(unsubscribe);

int mastodont_verify_credentials(mastodont_t* data, struct mstdnt_args* args,
                                 struct mstdnt_account* acct,
                                 struct mstdnt_storage* storage);
int mastodont_get_account(mastodont_t* data, struct mstdnt_args* args,
                          int lookup_type,
                          char* id,
                          struct mstdnt_account* acct,
                          struct mstdnt_storage* storage);

int mastodont_get_blocks(mastodont_t* data, struct mstdnt_args* args,
                         struct mstdnt_account_args* args,
                         struct mstdnt_storage* storage,
                         struct mstdnt_account* accts[],
                         size_t* accts_len);

int mastodont_get_mutes(mastodont_t* data, struct mstdnt_args* args,
                        struct mstdnt_account_args* args,
                        struct mstdnt_storage* storage,
                        struct mstdnt_account* accts[],
                        size_t* accts_len);

int mastodont_get_followers(mastodont_t* data, struct mstdnt_args* args,
                            char* id,
                            struct mstdnt_account_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_account* accts[],
                            size_t* accts_len);

int mastodont_get_following(mastodont_t* data, struct mstdnt_args* args,
                            char* id,
                            struct mstdnt_account_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_account* accts[],
                            size_t* accts_len);

int mstdnt_account_json(struct mstdnt_account* acct, cJSON* js);
int mstdnt_account_json_callback(cJSON* json, void* _args);
int mstdnt_accounts_json_callback(cJSON* json, void* _args);
void _mstdnt_val_account_call(cJSON* v, void* _type);
void _mstdnt_val_malloc_account_call(cJSON* v, void* _type);

// Cleanup
void mstdnt_cleanup_account(struct mstdnt_account* acct);
void mstdnt_cleanup_accounts(struct mstdnt_account* accts, size_t len);

#endif /* MASTODONT_ACCOUNT */
