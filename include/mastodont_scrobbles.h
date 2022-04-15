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

struct mstdnt_scrobble
{
    struct mstdnt_account account;
    char* album;
    char* artist;
    char* created_at;
    char* id;
    int length;
    char* title;
};

struct mstdnt_get_scrobbles_args
{
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
};

struct _mstdnt_scrobbles_cb_args
{
    struct mstdnt_scrobble** scrobbles;
    size_t* size;
};

int mastodont_get_scrobbles(mastodont_t* data,
                            char* id,
                            struct mstdnt_get_scrobbles_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_scrobble* scrobbles[],
                            size_t* size);

int mstdnt_scrobbles_result(struct mstdnt_fetch_results* results,
                            struct mstdnt_storage* storage,
                            struct mstdnt_scrobble* scrobbles[],
                            size_t* size);

int _mstdnt_scrobbles_result_callback(struct mstdnt_fetch_results* results,
                                      struct mstdnt_storage* storage,
                                      void* arg);

int mstdnt_scrobble_json(struct mstdnt_scrobble* scrobble, cJSON* js);

#endif /* MASTODONT_SCROBBLES_H */
