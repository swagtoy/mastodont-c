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

#include <string.h>
#include <stdlib.h>
#include <mastodont_list.h>
#include <mastodont_json_helper.h>
#include <mastodont_fetch.h>
#include <mastodont_request.h>
#include <mastodont_generate.h>

struct mstdnt_get_lists_args {
    struct mstdnt_list** lists;
    size_t* size;
};

int mstdnt_list_json(struct mstdnt_list* list, cJSON* js)
{
    cJSON* v;
    
    /* Zero out */
    memset(list, 0, sizeof(struct mstdnt_list));
    
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


GENERATE_JSON_ARRAY_FUNC(mstdnt_lists_json, struct mstdnt_list, mstdnt_list_json)
    
static int mstdnt_lists_json_callback(cJSON* json, void* _args)
{
    struct mstdnt_get_lists_args* args = _args;
    return mstdnt_lists_json(args->lists, args->size, json);
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
        mstdnt_lists_json_callback
    };

    return mastodont_request(data, &req_args);
}
