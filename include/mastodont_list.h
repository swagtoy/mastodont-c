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

#ifndef MASTODONT_LIST_H
#define MASTODONT_LIST_H
#include <mastodont_types.h>
#include <cjson/cJSON.h>
#include <mastodont_fetch.h>

struct mstdnt_list
{
    char* id;
    char* title;
    char* replies_policy;
};

int mstdnt_list_from_json(struct mstdnt_list* list, cJSON* js);

int mstdnt_lists_from_result(struct mstdnt_storage* storage,
                             struct mstdnt_fetch_results* results,
                             struct mstdnt_list* lists[],
                             size_t* size);

int mastodont_get_lists(mastodont_t* api,
                        struct mstdnt_list* lists[],
                        struct mstdnt_storage* storage,
                        size_t* size);

#endif /* MASTODONT_LIST_H */
