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
#include <mastodont_chats.h>
#include <mastodont_fetch.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

struct _mstdnt_chats_cb_args
{
    struct mstdnt_chat** chats;
    size_t* chats_len;
};

int mastodont_get_chats_v2(mastodont_t* data,
                           struct mstdnt_args* m_args,
                           struct mstdnt_chats_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_chat* chats[],
                           size_t* size)
{
    struct _mstdnt_chats_cb_args cb_args = { chats, size };
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_BOOL, "with_muted", { .b = args->with_muted } },
        { _MSTDNT_QUERY_STRING, "max_id", { .s = args->max_id } },
        { _MSTDNT_QUERY_STRING, "min_id", { .s = args->min_id } },
        { _MSTDNT_QUERY_STRING, "since_id", { .s = args->since_id } },
        { _MSTDNT_QUERY_INT, "limit", { .i = args->limit } },
        { _MSTDNT_QUERY_INT, "offset", { .i = args->offset } },
    };

    struct mastodont_request_args req_args = {
        .storage = storage,
        .url = "api/v2/pleroma/chats",
        .params_query = params,
        .params_query_len = _mstdnt_arg_len(params),
        .params_post = NULL,
        .params_post_len = 0,
        .request_type = CURLOPT_HTTPGET,
        .request_type_custom = NULL,
        .args = cb_args,
        .callback = mstdnt_chats_json_callback,
    };

    return mastodont_request(data, m_args, &req_args);
}
