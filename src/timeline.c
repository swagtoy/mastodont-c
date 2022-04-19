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

    union param_value u_local, u_remote, u_only_media,
        u_max_id, u_since_id, u_min_id, u_limit;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };

    struct mastodont_request_args req_args = {
        storage,
        url,
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback,
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
    
    union param_value u_local, u_remote, u_only_media,
        u_max_id, u_since_id, u_min_id, u_limit;
    u_local.i = args->local;
    u_remote.i = args->remote;
    u_only_media.i = args->only_media;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_INT, "local", u_local },
        { _MSTDNT_QUERY_INT, "remote", u_remote },
        { _MSTDNT_QUERY_INT, "only_media", u_only_media },
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/public",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback,
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
    
    union param_value u_max_id, u_since_id, u_min_id,
        u_limit, u_local, u_offset, u_with_muted;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_limit.i = args->limit;
    u_offset.i = args->offset;
    u_local.i = args->local;
    u_with_muted.i = args->with_muted;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
        { _MSTDNT_QUERY_INT, "local", u_local },
        { _MSTDNT_QUERY_INT, "with_muted", u_with_muted },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/direct",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback,
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
    
    union param_value u_max_id, u_since_id, u_min_id,
        u_limit, u_local, u_offset;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_limit.i = args->limit;
    u_local.i = args->local;
    u_offset.i = args->offset;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/timelines/home",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback,
    };
    
    return mastodont_request(data, &req_args);
}

