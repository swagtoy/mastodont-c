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

int mastodont_account(mastodont_t* data,
                      int lookup_type,
                      char* id,
                      struct mstdnt_account* acct,
                      struct mstdnt_storage* storage,
                      size_t* size);

int mstdnt_load_account_from_json(struct mstdnt_account* status, cJSON* js);


#endif /* MASTODONT_ACCOUNT */
