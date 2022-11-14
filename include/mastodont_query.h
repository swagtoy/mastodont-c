/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_QUERY_H
#define MASTODONT_QUERY_H
#include <stddef.h>
#include "mastodont_types.h"

enum _mstdnt_query_type
{
    _MSTDNT_QUERY_STRING,
    _MSTDNT_QUERY_INT,
    _MSTDNT_QUERY_BOOL,
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
        const char* s;
        int i;
        mstdnt_bool b;
        struct _mstdnt_query_array a;
        struct mstdnt_file* f;
    } value;
};

char* _mstdnt_query_string(mastodont_t* data, 
                           struct mstdnt_args* args,
                           char* src,
                           struct _mstdnt_query_param* params,
                           size_t param_len);

#endif /* MASTODONT_QUERY_H */
