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

#ifndef MASTODONT_SCROBBLES_H
#define MASTODONT_SCROBBLES_H
#include <stddef.h>
#include <mastodont.h>
#include <mastodont_types.h>
#include <mastodont_account.h>

typedef struct mstdnt_scrobble
{
    struct mstdnt_account account;
    char* album;
    char* artist;
    time_t created_at;
    char* id;
    int length;
    char* title;
} mstdnt_scrobble;

typedef struct mstdnt_scrobbles
{
    mstdnt_scrobble* scrobbles;
    size_t len;
} mstdnt_scrobbles;

struct mstdnt_get_scrobbles_args
{
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
};

// DEPRECATED 
struct _mstdnt_scrobbles_cb_args
{
    struct mstdnt_scrobble** scrobbles;
    size_t* size;
};

int
mstdnt_get_scrobbles(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* id,
                     struct mstdnt_get_scrobbles_args args);

int
mstdnt_scrobbles_json(struct mstdnt_scrobble* scrobbles[],
                      size_t* size,
                      cJSON* json);

int
mstdnt_scrobbles_json_callback(cJSON* json,
                               void* arg,
                               mstdnt_request_cb_data* data);

int
mstdnt_scrobble_json(struct mstdnt_scrobble* scrobble, cJSON* js);

void
mstdnt_cleanup_scrobble(mstdnt_scrobble* scrobble);

void
mstdnt_cleanup_scrobbles(mstdnt_scrobbles* scrobbles);

#endif /* MASTODONT_SCROBBLES_H */
