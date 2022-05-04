/*
 * Treebird - Lightweight frontend for Pleroma
 * Copyright (C) 2022 Nekobit
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MASTODONT_GENERATE_H
#define MASTODONT_GENERATE_H
#include <stdlib.h>
#include <cjson/cJSON.h>

#define GENERATE_JSON_ARRAY_FUNC(symbol, type, call_symbol) int symbol(type* array[], size_t* array_size, cJSON* js) { \
        if (!(array && cJSON_IsArray(js))) return 1;                    \
        size_t i = 0;                                                   \
        cJSON *arr_j_list;                                              \
        size_t arr_len = cJSON_GetArraySize(js);                        \
        if (array_size) *array_size = arr_len;                          \
                                                                        \
        if (!arr_len)                                                   \
            return 0;                                                   \
                                                                        \
        *array = calloc(1, arr_len * sizeof(type));                     \
                                                                        \
        if (*array == NULL)                                             \
            return 1;                                                   \
        cJSON_ArrayForEach(arr_j_list, js)                              \
        {                                                               \
            call_symbol((*array) + i++, arr_j_list->child);             \
        }                                                               \
        return 0;                                                       \
    }
        

#endif // MASTODONT_GENERATE_H
