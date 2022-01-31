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

#ifndef MASTODONT_JSON_HELPER_H
#define MASTODONT_JSON_HELPER_H
#include "mastodont_types.h"

#define _mstdnt_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))

struct _mstdnt_str_val
{
    const char* key;
    char** key_ptr;
};

struct _mstdnt_bool_val
{
    const char* key;
    mstdnt_bool* bool_ptr;
};

int _mstdnt_key_val_iter(cJSON* v,
                         struct _mstdnt_str_val* str,
                         size_t str_len,
                         struct _mstdnt_bool_val* bools,
                         size_t bool_len);

#endif /* MASTODONT_JSON_HELPER_H */
