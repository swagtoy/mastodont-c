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

#ifndef MASTODONT_QUERY_H
#define MASTODONT_QUERY_H
#include <stddef.h>
#include "mastodont_types.h"

enum _mstdnt_query_type
{
    _MSTDNT_QUERY_STRING,
    _MSTDNT_QUERY_INT,
    _MSTDNT_QUERY_ARRAY,
    _MSTDNT_QUERY_FILE,
};

struct _mstdnt_query_array
{
    char** arr;
    size_t arr_len;
};

struct _mstdnt_query_param
{
    enum _mstdnt_query_type type;
    char* key;
    union param_value {
        char* s;
        int i;
        struct _mstdnt_query_array a;
        struct mstdnt_file* f;
    } value;
};

char* _mstdnt_query_string(mastodont_t* data,
                           char* src,
                           struct _mstdnt_query_param* params,
                           size_t param_len);

#endif /* MASTODONT_QUERY_H */
