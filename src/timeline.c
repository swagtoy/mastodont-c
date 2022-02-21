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
#include "mastodont_fetch.h"
#include "mastodont_timeline.h"
#include "mastodont_query.h"

int mastodont_timeline_public(mastodont_t* data,
                              struct mstdnt_timeline_public_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* size)
{
    int res;
    struct mstdnt_fetch_results results = { 0 };
    
    /* Default args */
    struct mstdnt_timeline_public_args _args;
    if (args == NULL)
    {
        _args.local = 0; /* Defaults to false */
        _args.remote = 0;
        _args.only_media = 0;
        _args.max_id = NULL;
        _args.since_id = NULL;
        _args.min_id = NULL;
        _args.limit = 20;
        args = &_args;
    }
    storage->needs_cleanup = 0;

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
    
    char* url = _mstdnt_query_string("api/v1/timelines/public", params, _mstdnt_arr_len(params));

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_HTTPGET) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }

    res = mstdnt_load_statuses_from_result(statuses, storage, &results, size);

    mastodont_fetch_results_cleanup(&results);
    
cleanup:
    free(url);
    return res;
}
