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

#include <mastodont_json_helper.h>

int _mstdnt_json_init(cJSON** root,
                      struct mstdnt_fetch_results* results,
                      struct mstdnt_storage* storage)
{
    *root = cJSON_ParseWithLength(results->response, results->size);
    if (*root == NULL)
        return 1;
    storage->root = *root;
    storage->needs_cleanup = 1;
    return 0;
}

int _mstdnt_key_val_iter(cJSON* v,
                         struct _mstdnt_str_val* str,
                         size_t str_len,
                         struct _mstdnt_bool_val* bools,
                         size_t bool_len)
{
    size_t i;
    if (str && cJSON_IsString(v))
    {
        for (i = 0; i < str_len; ++i)
        {
            if (strcmp(str[i].key, v->string) == 0)
            {
                *(str[i].key_ptr) = v->valuestring;
                return 0;
            }
        }
    }
    if (bools && cJSON_IsBool(v))
    {
        for (i = 0; i < bool_len; ++i)
        {
            if (strcmp(bools[i].key, v->string) == 0)
            {
                *(bools[i].bool_ptr) = cJSON_IsTrue(v) ? cJSON_True : cJSON_False;
                return 0;
            }
        }
    }
    return 1;
}
