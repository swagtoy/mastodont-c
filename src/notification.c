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

int mastodont_get_notifications(mastodont_t* data,
                                struct mstdnt_get_notifications_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_notification** notifs,
                                size_t* size)
{
    int res = 0;
    struct mstdnt_fetch_results results = { 0 };
    
    /* Default args */
    storage->needs_cleanup = 0;

    union param_value u_account_id, u_with_muted, u_max_id,
        u_min_id, u_since_id, u_offset, u_limit;
    u_account_id.s = args->account_id;
    u_with_muted.i = args->with_muted;
    u_max_id.s = args->max_id;
    u_min_id.s = args->min_id;
    u_since_id.s = args->since_id;
    u_offset.i = args->offset;
    u_limit.i = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "account_id", u_account_id },
        { _MSTDNT_QUERY_INT, "with_muted", u_with_muted },
        { _MSTDNT_QUERY_STRING, "max_id", u_max_id },
        { _MSTDNT_QUERY_STRING, "min_id", u_min_id },
        { _MSTDNT_QUERY_STRING, "since_id", u_since_id },
        { _MSTDNT_QUERY_INT, "offset", u_offset },
        { _MSTDNT_QUERY_INT, "limit", u_limit },
    };

    char* post = _mstdnt_query_string(data, NULL, params, _mstdnt_arr_len(params));

    curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);

    if (mastodont_fetch_curl(data, "api/v1/apps", &results, CURLOPT_POST) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }
/*
    if (mstdnt_check_error(&results, storage))
    {
        res = 1;
        goto cleanup_fetch;
    }
*/

/*    res = mstdnt_read_app_result(storage, &results, app);*/

cleanup_fetch:
    mastodont_fetch_results_cleanup(&results);
cleanup:
    free(post);
    return res;    
}

int mstdnt_load_notification_from_json(struct mstdnt_notification* notif, cJSON* js)
{
    
}
