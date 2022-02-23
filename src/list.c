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

#include <stdlib.h>
#include <mastodont_list.h>
#include <mastodont_json_helper.h>
#include <mastodont_fetch.h>

int mstdnt_load_list_from_json(struct mstdnt_list* list, cJSON* js)
{
    cJSON* v;
    struct _mstdnt_str_val strings[] = {
        { "id", &(list->id) },
        { "title", &(list->title) },
        { "replies_policy", &(list->replies_policy) }
    };
    
    for (v = js; v; v = v->next)
        if (_mstdnt_key_val_iter(v, strings, _mstdnt_arr_len(strings),
                                 NULL, 0) == 1)
            return 1;
}


int mstdnt_load_lists_from_result(struct mstdnt_list* lists[],
                                  struct mstdnt_storage* storage,
                                  struct mstdnt_fetch_results* results,
                                  size_t* size)
{
    size_t i = 0;
    cJSON* root, *list_j_list;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    if (!cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *lists = malloc((size ? *size : cJSON_GetArraySize(root))
                    * sizeof(struct mstdnt_list));
    if (*lists == NULL)
        return 1;
    
    cJSON_ArrayForEach(list_j_list, root)
    {
        mstdnt_load_list_from_json((*lists) + i++, list_j_list->child);
    }
    storage->needs_cleanup = 1;
    return 0;
}

int mastodont_get_lists(mastodont_t* data,
                        struct mstdnt_list* lists[],
                        struct mstdnt_storage* storage,
                        size_t* size)
{
    int res = 0;
    cJSON* root;
    struct mstdnt_fetch_results results = { 0 };
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, "api/v1/lists",
                             &results, CURLOPT_HTTPGET) != CURLE_OK)
    {
        return 1;
    }

    // TODO
cleanup:
    return res;
}
