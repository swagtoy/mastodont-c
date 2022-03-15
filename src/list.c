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
#include <mastodont_request.h>

struct mstdnt_get_lists_args {
    struct mstdnt_list** lists;
    size_t* size;
};

int mstdnt_list_from_json(struct mstdnt_list* list, cJSON* js)
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
    return 0;
}

int mstdnt_lists_from_result(struct mstdnt_storage* storage,
                             struct mstdnt_fetch_results* results,
                             struct mstdnt_list* lists[],
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
        mstdnt_list_from_json((*lists) + i++, list_j_list->child);
    }
    storage->needs_cleanup = 1;
    return 0;
}

static int mstdnt_lists_from_result_callback(struct mstdnt_fetch_results* results,
                                             struct mstdnt_storage* storage,
                                             void* _args)
{
    struct mstdnt_get_lists_args* args = _args;
    return mstdnt_lists_from_result(storage, results, args->lists, args->size);
}

int mastodont_get_lists(mastodont_t* data,
                        struct mstdnt_list* lists[],
                        struct mstdnt_storage* storage,
                        size_t* size)
{
    struct mstdnt_get_lists_args args = {
        lists,
        size
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/lists",
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        &args,
        mstdnt_lists_from_result_callback
    };

    return mastodont_request(data, &req_args);
}
