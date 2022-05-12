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
#include <mastodont_fetch.h>
#include <mastodont_timeline.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

int mastodont_timeline_list(mastodont_t* data,
                            char* list_id,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/timelines/list/%s", list_id);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        mstdnt_statuses_json_callback,
    };

    return mastodont_request(data, &req_args);
}

int mastodont_timeline_tag(mastodont_t* data,
                           char* hashtag,
                           struct mstdnt_timeline_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_status* statuses[],
                           size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "api/v1/timelines/tag/%s", hashtag);
    
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "local", { .i = args->local } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
        { _MSTDNT_QUERY_INT, "remote", { .i = args->remote } },
        { _MSTDNT_QUERY_INT, "only_media", { .i = args->remote } },
        { _MSTDNT_QUERY_INT, "with_muted", { .i = args->remote } },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        mstdnt_statuses_json_callback,
    };

    return mastodont_request(data, &req_args);
}

int mastodont_timeline_public(mastodont_t* data,
                              struct mstdnt_timeline_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_INT, "local", { .i = args->local } },
        { _MSTDNT_QUERY_INT, "remote", { .i = args->remote } },
        { _MSTDNT_QUERY_INT, "only_media", { .i = args->only_media } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/public",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        mstdnt_statuses_json_callback,
    };
    
    return mastodont_request(data, &req_args);
}


int mastodont_timeline_direct(mastodont_t* data,
                              struct mstdnt_timeline_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
        { _MSTDNT_QUERY_INT, "local", { .i = args->local } },
        { _MSTDNT_QUERY_INT, "with_muted", { .i = args->with_muted } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/direct",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        mstdnt_statuses_json_callback,
    };
    
    return mastodont_request(data, &req_args);
}


int mastodont_timeline_home(mastodont_t* data,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/home",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        mstdnt_statuses_json_callback,
    };
    
    return mastodont_request(data, &req_args);
}

