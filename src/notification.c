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

#include <stdlib.h>
#include <mastodont_notification.h>
#include <mastodont_fetch.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>

int mstdnt_notification_from_json(struct mstdnt_notification* notif, cJSON* js)
{
    
}



int mastodont_get_notifications(mastodont_t* data,
                                struct mstdnt_get_notifications_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_notification** notifs,
                                size_t* size)
{
    struct _mstdnt_statuses_cb_args cb_args = { statuses, size };
    
    union param_value u_exclude_types, u_account_id, u_exclude_visibilities,
        u_include_types, u_with_muted, u_max_id, u_min_id,
        u_since_id, u_offset, u_limit;
    /* TODO Arrays of excludes, includes */
    u_account_id.s = args->account_id;
    u_with_muted.i = args->with_muted;
    u_max_id.s = args->max_id;
    u_since_id.s = args->since_id;
    u_min_id.s = args->min_id;
    u_offset.i = args->offset;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "account_id", u_account_id },
        { _MSTDNT_QUERY_INT, "with_muted", u_with_muted },
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/notifications",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_statuses_result_callback,
    };
    
    return mastodont_request(data, &req_args);
}
