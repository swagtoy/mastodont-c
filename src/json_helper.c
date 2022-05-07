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

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <mastodont_json_helper.h>

// UNIX timestamp, above but I don't care
#define TIMESTAMP_LEN 16

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

int _mstdnt_key_val_ref(cJSON* v, struct _mstdnt_val_ref* refs,
                        size_t refs_len)
{
    size_t i;
    for (i = 0; i < refs_len; ++i)
    {
        if (v->string && strcmp(refs[i].key, v->string) == 0)
        {
            refs[i].handle(v, refs[i].val);
            return 0;
        }
    }
    return 1;
}

void _mstdnt_val_string_unix_call(cJSON* v, void* _type)
{
    long conv;
    time_t* type = _type;
    char* endptr;
    if (!cJSON_IsString(v))
    {
        *type = 0;
        return;
    }

    // Convert string to long
    conv = strtol(v->valuestring, &endptr, 10);
    *type = v->valuestring != endptr ? conv : 0;
}

// Fuck you Gargron
void _mstdnt_val_string_uint_call(cJSON* v, void* _type)
{
    long conv;
    unsigned* type = _type;
    char* endptr;
    if (!cJSON_IsString(v))
    {
        *type = 0;
        return;
    }

    // Convert string to long
    conv = strtol(v->valuestring, &endptr, 10);
    *type = v->valuestring != endptr ? conv : 0;
}

// Again, fuck you Gargron
void _mstdnt_val_string_int_call(cJSON* v, void* _type)
{
    long conv;
    int* type = _type;
    char* endptr;
    if (!cJSON_IsString(v))
    {
        *type = 0;
        return;
    }

    // Convert string to long
    conv = strtol(v->valuestring, &endptr, 10);
    *type = v->valuestring != endptr ? conv : 0;
}

void _mstdnt_val_string_call(cJSON* v, void* _type)
{
    char** type = _type;
    if (!cJSON_IsString(v))
    {
        *type = NULL;
        return;
    }
    *type = v->valuestring;
}

void _mstdnt_val_bool_call(cJSON* v, void* _type)
{
    mstdnt_bool* type = _type;
    if (!cJSON_IsBool(v))
    {
        *type = -1;
        return;
    }
    *type = cJSON_IsTrue(v);
}

void _mstdnt_val_uint_call(cJSON* v, void* _type)
{
    unsigned* type = _type;
    if (!cJSON_IsNumber(v))
    {
        *type = 0;
        return;
    }
    *type = v->valueint;
}


void _mstdnt_val_sint_call(cJSON* v, void* _type)
{
    int* type = _type;
    if (!cJSON_IsNumber(v))
    {
        *type = 0;
        return;
    }
    *type = v->valueint;
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
                *(bools[i].bool_ptr) = cJSON_IsTrue(v);
                return 0;
            }
        }
    }
    return 1;
}
