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

#include <mastodont_notification.h>

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

    union param_value
        u_account_id = args->account_id,
        u_with_muted = args->with_muted,
        u_max_id = args->max_id,
        u_min_id = args->min_id,
        u_since_id = args->since_id,
        u_offset = args->offset,
        u_limit = args->limit;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "client_name", u_client_name },
        { _MSTDNT_QUERY_STRING, "redirect_uris", u_redirect_uris },
        { _MSTDNT_QUERY_STRING, "scopes", u_scopes },
        { _MSTDNT_QUERY_STRING, "website", u_website },
    };

    char* post = _mstdnt_query_string(NULL, params, _mstdnt_arr_len(params));

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

    res = mstdnt_read_app_result(storage, &results, app);

cleanup_fetch:
    mastodont_fetch_results_cleanup(&results);
cleanup:
    free(post);
    return res;    
}

int mstdnt_load_account_from_json(struct mstdnt_notification* notif, cJSON* js)
{
    
}
