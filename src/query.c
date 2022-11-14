/*
 * Licensed under BSD 2-Clause License
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_query.h>

#define CONV_SIZE 64

/* TODO audit this function for overflows */
char* _mstdnt_query_string(mastodont_t* data,
                           struct mstdnt_args* args,
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
    char* result = mstdnt_malloc(res_len);
    if (src_l)
        strcpy(result, src);

    /* We'll call them res to represent the query parameters */
    int res_count = 0;
    size_t arr_ind = 0;
    char* key_ptr;
    int invalid_bool;

    /* If it's an array, we treat it as a "fake" regular key and keep iterating through it */
    for (i = 0; i < param_len; ++i)
    {
        invalid_bool = 0;
        escape_str = NULL;

        /* If array is NULL, skip */
        if (params[i].type == _MSTDNT_QUERY_ARRAY &&
            !(params[i].value.a.arr && params[i].value.a.arr_len))
            continue;
        
        /* Start up array */
        if (params[i].type == _MSTDNT_QUERY_ARRAY && arr_ind == 0)
        {
            size_t str_s = strlen(params[i].key);
            key_ptr = mstdnt_malloc(str_s+3); /* 2 "[]" + 1 \0 */
            strcpy(key_ptr, params[i].key);
            strcpy(key_ptr+str_s, "[]");
        }

        if (params[i].type != _MSTDNT_QUERY_ARRAY)
        {
            key_ptr = params[i].key;
        }

        // If boolean and set to 0, make it unset
        if (params[i].type == _MSTDNT_QUERY_BOOL &&
            !(params[i].value.b >= 1))
            invalid_bool = 1;

        // If invalid_bool is true, then it's not actually set
        if (key_ptr &&
            !(params[i].type == _MSTDNT_QUERY_STRING &&
              params[i].value.s == NULL) &&
            !(params[i].type == _MSTDNT_QUERY_BOOL &&
              invalid_bool))
        {

            if (res_count++ == 0 && src_l)
                /* Replaces Null terminator */
                result[res_len-1] = '?';

            /* Convert value */
            if (params[i].type == _MSTDNT_QUERY_BOOL)
            {
                if (params[i].value.b == 1)
                    val_ptr = "false";
                else // Anything above 1, can't be zero though
                    val_ptr = "true";
            }
            else if (params[i].type == _MSTDNT_QUERY_INT) {
                snprintf(conv_val, CONV_SIZE, "%d", params[i].value.i);
                val_ptr = conv_val;
            }
            else if (params[i].type == _MSTDNT_QUERY_ARRAY) {
                val_ptr = params[i].value.a.arr[arr_ind];
            }
            else { /* Point to it, it's a string */
                /* First, let's encode it */
                escape_str = MSTDNT_T_FLAG_ISSET(args, MSTDNT_FLAG_NO_URI_SANITIZE) ?
                    params[i].value.s : curl_easy_escape(data->curl, params[i].value.s, 0);
                val_ptr = escape_str;
            }

            if (val_ptr == NULL)
                break;

            /* Get lengths */
            key_len = strlen(key_ptr);
            val_len = strlen(val_ptr);

            res_prev = res_len;
            /*        |v| & character |v| account for '=' */
            res_len += 1 + key_len +   1 + val_len;


            result = mstdnt_realloc(result, res_len + 1); /* NULL terminator space */
            if (res_count - 1 != 0)
                result[res_prev-1] = '&';

            /* Leave an extra byte, doesn't hurt */
            if (res_count - 1 != 0)
                result[res_len-1] = '\0';
            else result[res_len] = '\0';

            /* Copy over strings (skip & sign ;; +1) */
            strcpy(result + res_prev, key_ptr);
            result[res_prev + key_len] = '=';
            strcpy(result + res_prev + 1 + key_len, val_ptr);
            /* Only free if flag is set, meaning it needs to be free'd */
            if (!MSTDNT_T_FLAG_ISSET(args, MSTDNT_FLAG_NO_URI_SANITIZE))
                curl_free(escape_str);
        }

        /* Finish array stuff */
        if (params[i].type == _MSTDNT_QUERY_ARRAY)
        {
            ++arr_ind;

            if (arr_ind >= params[i].value.a.arr_len)
            {
                arr_ind = 0;
                mstdnt_free(key_ptr);
            }
            else {
                --i; /* Flip flop i */
            }
        }
    }

    return result;
}
