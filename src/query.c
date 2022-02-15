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
#include <mastodont_query.h>

#define CONV_SIZE 64

char* _mstdnt_query_string(char* src,
                           struct _mstdnt_query_param* params,
                           size_t param_len)
{
    size_t i;
    int res_prev;
    /* If value type is an int, convert it with int->str */
    char conv_val[CONV_SIZE];
    size_t src_l = strlen(src);

    /* Result */
    size_t res_len = src_l+1;
    char* result = malloc(res_len);
    strncpy(result, src, res_len);

    /* We'll call them res to represent the query parameters */
    int res_count = 0;

    for (i = 0; i < param_len; ++i)
    {
        if (params[i].key)
        {
            if (res_count++ == 0)
            {
                result = realloc(result, ++res_len);
                result[res_len-1] = '?';
            }

            /* Convert value */
            if (params[i].type == _MSTDNT_QUERY_INT)
                snprintf(conv_val, CONV_SIZE, "%d", params[i].value.i);
            else /* Point to it */
                conv_val = params[i].value.s;
            
            res_prev = res_len;
            /*        |v| & character             |v| account for '=' */
            res_len += 1 + strlen(params[i].key) + 1 + strlen(conv_val);

            
            result = realloc(result, res_len + 1);
            if (res_count) result[res_prev] = '&';
            
            if (res_count) result[res_len-2] = '\0';
            else result[res_len-1] = '\0';

            /* TODO */
        }
    }

    return result;
}
