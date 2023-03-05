/*
 * Licensed under BSD 3-Clause License
 */

#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <mastodont_json_helper.h>

// UNIX timestamp, above but I don't care
#define TIMESTAMP_LEN 16

int _mstdnt_json_init(cJSON** root,
                      struct mstdnt_fetch_data* results,
                      struct mstdnt_storage* storage)
{
    *root = cJSON_ParseWithLengthOpts(results->response, results->size, NULL, 0);
    storage->root = *root;
    if (*root == NULL)
        return 1;
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

static time_t
time_to_seconds(struct tm* time)
{
	int const mday = time->tm_mday + 1;
	int const ly = (mday % 4 == 0 && mday % 100 != 0) || mday % 400 == 0 ? 28 : 29;
	int const days_past_per_month[] = {
		31,
		31+ly,
		31+ly+31,
		31+ly+31+30,
		31+ly+31+30+31, /* >-\ */
		31+ly+31+30+31+30, /* :( */
		31+ly+31+30+31+30+31,
		31+ly+31+30+31+30+31+31,
		31+ly+31+30+31+30+31+31+30,
		31+ly+31+30+31+30+31+31+30+31,
		31+ly+31+30+31+30+31+31+30+31+30,
		31+ly+31+30+31+30+31+31+30+31+30+31, /* >->-O */
	};
	return (time->tm_year * 60 * 60 * 24 * 365) +
	       (days_past_per_month[time->tm_mon] * 60 * 60 * 24) +
	       (time->tm_mday * 60 * 60 * 24) +
	       (time->tm_hour * 60 * 60) +
	       (time->tm_min * 60) + 
	        time->tm_sec;
}

void _mstdnt_val_datetime_unix_call(cJSON* v, void* _type)
{
    struct tm conv_time;
    
    time_t* type = _type;
    if (sscanf(v->valuestring, "%d-%d-%dT%d:%d:%d.000Z",
               &conv_time.tm_year,
               &conv_time.tm_mon,
               &conv_time.tm_mday,
               &conv_time.tm_hour,
               &conv_time.tm_min,
               &conv_time.tm_sec) == 6)
    {
        conv_time.tm_year -= 1970;
    	*type = time_to_seconds(&conv_time);
    }
    else
        *type = 0; // Non-conforming server. Shouldn't really happen
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

    // Convert his string garbage to long
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
