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

/* TODO audit this function for overflows */
char* _mstdnt_query_string(mastodont_t* data,
                           char* src,
                           struct _mstdnt_query_param* params,
                           size_t param_len)
{
    size_t i;
    int res_prev;
    char* escape_str;
    /* If value type is an int, convert it with int->str */
    char* val_ptr = NULL;
    char conv_val[CONV_SIZE];
    size_t src_l;
    if (src)
        src_l = strlen(src);
    else
        src_l = 0;

    /* Key values */
    size_t key_len;
    size_t val_len;

    /* Result */
    size_t res_len;
    if (src_l)
        res_len = src_l+1;
    else
        res_len = 0;
    char* result = malloc(res_len);
    if (src_l)
        strncpy(result, src, res_len);

    /* We'll call them res to represent the query parameters */
    int res_count = 0;

    for (i = 0; i < param_len; ++i)
    {
        escape_str = NULL;
        if (params[i].key &&
            !(params[i].type == _MSTDNT_QUERY_STRING &&
              params[i].value.s == NULL))
        {

            if (res_count++ == 0 && src_l)
                /* Replaces Null terminator */
                result[res_len-1] = '?';

            /* Convert value */
            if (params[i].type == _MSTDNT_QUERY_INT)
            {
                snprintf(conv_val, CONV_SIZE, "%d", params[i].value.i);
                val_ptr = conv_val;
            }
            else /* Point to it, it's a string */
            {
                /* First, let's encode it */
                escape_str = MSTDNT_T_FLAG_ISSET(data, MSTDNT_FLAG_NO_URI_SANITIZE) ?
                    params[i].value.s : curl_easy_escape(data->curl, params[i].value.s, 0);
                val_ptr = escape_str;
            }

            /* Get lengths */
            key_len = strlen(params[i].key);
            val_len = strlen(val_ptr);

            res_prev = res_len;
            /*        |v| & character |v| account for '=' */
            res_len += 1 + key_len +   1 + val_len;


            result = realloc(result, res_len + 1); /* NULL terminator space */
            if (res_count - 1 != 0)
                result[res_prev-1] = '&';

            /* Leave an extra byte, doesn't hurt */
            if (res_count - 1 != 0)
                result[res_len-1] = '\0';
            else result[res_len] = '\0';

            /* Copy over strings (skip & sign ;; +1) */
            strcpy(result + res_prev, params[i].key);
            result[res_prev + key_len] = '=';
            strcpy(result + res_prev + 1 + key_len, val_ptr);
            /* Only free if flag is set, meaning it needs to be free'd */
            if (!MSTDNT_T_FLAG_ISSET(data, MSTDNT_FLAG_NO_URI_SANITIZE))
                curl_free(escape_str);
        }
    }
    

    return result;
}
