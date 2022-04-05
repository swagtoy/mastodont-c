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
#include <cjson/cJSON.h>

#define MSTDNT_LOOKUP_ACCT 0
#define MSTDNT_LOOKUP_ID 1

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

#define MSTDNT_ACCOUNT_ACTION_DECL(type) int mastodont_##type##_account(mastodont_t* data, char* id, struct mstdnt_storage* storage, struct mstdnt_account* acct)
#define MSTDNT_ACCOUNT_ACTION_FUNC_URL(action) { \
    return mstdnt_account_action(data, id, storage, acct, "api/v1/accounts/%s/" action);\
    }

MSTDNT_ACCOUNT_ACTION_DECL(follow);
MSTDNT_ACCOUNT_ACTION_DECL(unfollow);
MSTDNT_ACCOUNT_ACTION_DECL(mute);
MSTDNT_ACCOUNT_ACTION_DECL(unmute);
MSTDNT_ACCOUNT_ACTION_DECL(block);
MSTDNT_ACCOUNT_ACTION_DECL(unblock);
MSTDNT_ACCOUNT_ACTION_DECL(subscribe);
MSTDNT_ACCOUNT_ACTION_DECL(unsubscribe);

int mstdnt_account_from_result(struct mstdnt_fetch_results* results,
                               struct mstdnt_storage* storage,
                               struct mstdnt_account* acct,
                               size_t* size);

int mastodont_get_account(mastodont_t* data,
                          int lookup_type,
                          char* id,
                          struct mstdnt_account* acct,
                          struct mstdnt_storage* storage,
                          size_t* size);

int mstdnt_account_from_json(struct mstdnt_account* status, cJSON* js);

void _mstdnt_val_account_call(cJSON* v, void* _type);
void _mstdnt_val_malloc_account_call(cJSON* v, void* _type);

#endif /* MASTODONT_ACCOUNT */
