/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_JSON_HELPER_H
#define MASTODONT_JSON_HELPER_H
#include "mastodont_types.h"
#include "mastodont_fetch.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _mstdnt_generic_args
{
    void* arg;
    size_t* size;
};

struct _mstdnt_val_ref
{
    const char* key;
    void* val;
    void (*handle)(cJSON*, void*);
};

int _mstdnt_json_init(cJSON** root,
                      struct mstdnt_fetch_data* results,
                      struct mstdnt_storage* storage);

int _mstdnt_key_val_ref(cJSON* v, struct _mstdnt_val_ref* refs,
                        size_t refs_len);

void _mstdnt_val_string_int_call(cJSON* v, void* _type);
void _mstdnt_val_string_uint_call(cJSON* v, void* _type);
void _mstdnt_val_string_unix_call(cJSON* v, void* _type);
void _mstdnt_val_string_call(cJSON* v, void* _type);
void _mstdnt_val_bool_call(cJSON* v, void* _type);
void _mstdnt_val_uint_call(cJSON* v, void* _type);
void _mstdnt_val_sint_call(cJSON* v, void* _type);
void _mstdnt_val_datetime_unix_call(cJSON* v, void* _type);

/* DEPRECATED */
struct _mstdnt_str_val
{
    const char* key;
    char** key_ptr;
};

/* DEPRECATED */
struct _mstdnt_bool_val
{
    const char* key;
    mstdnt_bool* bool_ptr;
};

/* DEPRECATED */
int _mstdnt_key_val_iter(cJSON* v,
                         struct _mstdnt_str_val* str,
                         size_t str_len,
                         struct _mstdnt_bool_val* bools,
                         size_t bool_len);

#ifdef __cplusplus
}
#endif
						 
#endif /* MASTODONT_JSON_HELPER_H */
