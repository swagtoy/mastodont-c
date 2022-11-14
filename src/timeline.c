/*
 * Licensed under BSD 2-Clause License
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_fetch.h>
#include <mastodont_timeline.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

int
mstdnt_timeline_list(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* list_id,
                     struct mstdnt_timeline_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/timelines/list/%s", list_id);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "local", { .b = args.local } },
        { _MSTDNT_QUERY_BOOL, "remote", { .b = args.remote } },
        { _MSTDNT_QUERY_BOOL, "only_media", { .b = args.only_media } },
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
        /* { _MSTDNT_QUERY_INT, "exclude_visibilities", { .i = args.only_media } }, */
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
    };

    struct mstdnt_request_args req_args = {
        .url = url,
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_statuses_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int
mstdnt_timeline_tag(mastodont_t* data,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    char* hashtag,
                    struct mstdnt_timeline_args args)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/timelines/tag/%s", hashtag);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        // TODO ANY
        // TODO ALL
        // TODO NONE
        // TODO exclude_visibilities
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_BOOL, "local", { .b = args.local } },
        { _MSTDNT_QUERY_BOOL, "remote", { .b = args.remote } },
        { _MSTDNT_QUERY_BOOL, "only_media", { .b = args.only_media } },
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
    };

    struct mstdnt_request_args req_args = {
        .url = url,
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_statuses_json_callback,
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

static const char*
reply_visibility_str(enum mstdnt_reply_visibility vis)
{
    switch (vis)
    {
    case MSTDNT_REPLY_VIS_SELF:
        return "self";
    case MSTDNT_REPLY_VIS_FOLLOWING:
        return "following";
    case MSTDNT_REPLY_VIS_NONE:
    default:
        return NULL;
    }
}

int
mstdnt_timeline_public(mastodont_t* data,
                       struct mstdnt_args* m_args,
                       mstdnt_request_cb_t cb_request,
                       void* cb_args,
                       struct mstdnt_timeline_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "local", { .b = args.local } },
        { _MSTDNT_QUERY_STRING, "instance", { .s = args.instance } },
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
        { _MSTDNT_QUERY_STRING, "reply_visibility", { .s = (char*)reply_visibility_str(args.reply_visibility) } },
        { _MSTDNT_QUERY_BOOL, "remote", { .b = args.remote } },
        { _MSTDNT_QUERY_BOOL, "only_media", { .b = args.only_media } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
    };

    struct mstdnt_request_args req_args = {
        .url = "api/v1/timelines/public",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_statuses_json_callback,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}


int
mstdnt_timeline_direct(mastodont_t* data,
                       struct mstdnt_args* m_args,
                       mstdnt_request_cb_t cb_request,
                       void* cb_args,
                       struct mstdnt_timeline_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args.with_muted } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/timelines/direct",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_statuses_json_callback,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}


int
mstdnt_timeline_home(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     struct mstdnt_timeline_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "local", { .b = args.local } },
        { _MSTDNT_QUERY_BOOL, "remote", { .b = args.remote } },
        { _MSTDNT_QUERY_INT, "only_media", { .i = args.only_media } },
        { _MSTDNT_QUERY_INT, "with_muted", { .i = args.with_muted } },
        /* { _MSTDNT_QUERY_INT, "exclude_visibilities", { .i = args.with_muted } }, */
        { _MSTDNT_QUERY_STRING, "reply_visibility", { .s = (char*)reply_visibility_str(args.reply_visibility) } },        
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args.max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args.since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args.min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args.limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args.offset } },
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/timelines/home",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_statuses_json_callback,
    };
    
    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

