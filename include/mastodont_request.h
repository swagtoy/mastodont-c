/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_REQUEST_H
#define MASTODONT_REQUEST_H
#include <cjson/cJSON.h>
#include "mastodont_types.h"
#include "mastodont_fetch.h"
#include "mastodont_query.h"

struct mstdnt_request_args
{
    struct mstdnt_storage* storage;
    char* url;
    struct _mstdnt_query_param* params_query;
    size_t params_query_len;
    struct _mstdnt_query_param* params_post;
    size_t params_post_len;
    CURLoption request_type;
    char* request_type_custom;
    void* args;
    int (*callback)(cJSON*, void*, mstdnt_request_cb_data*);
};

/**
 * Sends a mastoAPI request to an HTTP server.
 *
 * @param data Mastodont struct
 * @param m_args Mastodont general args pointer
 * @param cb_request Callback pointer associated with this request
 * @param cb_args Args passed into cb_request
 * @param args Propagated arguments to request to the server
 */
int mstdnt_request(mastodont_t* data,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   struct mstdnt_request_args* args);

#endif /* MASTODONT_REQUEST_H */
