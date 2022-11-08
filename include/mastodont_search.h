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

#ifndef MASTODONT_SEARCH_H
#define MASTODONT_SEARCH_H
#include "mastodont.h"
#include "mastodont_types.h"

enum mstdnt_search_type
{
    MSTDNT_SEARCH_ACCOUNTS = 1,
    MSTDNT_SEARCH_HASHTAGS,
    MSTDNT_SEARCH_STATUSES,
};

struct mstdnt_search_args
{
    char* account_id;
    char* max_id;
    char* since_id;
    char* min_id;
    enum mstdnt_search_type type;
    mstdnt_bool with_relationships;
    mstdnt_bool exclude_unreviewed;
    mstdnt_bool resolve;
    int limit;
    int offset;
    mstdnt_bool following;
};

struct mstdnt_search_results
{
    struct mstdnt_accounts* accts;
    struct mstdnt_statuses* statuses;
    struct mstdnt_tags* tags;
};

int
mstdnt_search_json_callback(cJSON* json,
                            void* _args,
                            mstdnt_request_cb_data* data);

int
mstdnt_search_json(struct mstdnt_search_results* search_results, cJSON* json);

int
mstdnt_search(mastodont_t* data,
              struct mstdnt_args* m_args,
              mstdnt_request_cb_t cb_request,
              void* cb_args,
              char* query,
              struct mstdnt_search_args args);

void mstdnt_cleanup_search_results(struct mstdnt_search_results* res);

#endif /* MASTODONT_SEARCH_H */
