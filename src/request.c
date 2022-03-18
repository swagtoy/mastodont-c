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
#include <assert.h>
#include <mastodont_request.h>
#include <mastodont_query.h>

int mastodont_request(mastodont_t* data, struct mastodont_request_args* args)
{
    int res = 0;
    struct mstdnt_storage* storage = args->storage;
    struct mstdnt_fetch_results results = { 0 };
    char* post;
    char* url_query = args->params_query ?
        _mstdnt_query_string(data, args->url, args->params_query, args->params_query_len) :
        args->url;

    storage->needs_cleanup = 0;

    if (args->params_post)
    {
        post = _mstdnt_query_string(data, NULL, args->params_post, args->params_post_len);
        curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);
    }

    if (mastodont_fetch_curl(data, url_query, &results, args->request_type) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }

    if (mstdnt_check_error(&results, storage))
    {
        res = 1;
        goto cleanup_res;
    }

    /* Optional */
    if (args->callback) args->callback(&results, storage, args->args);

cleanup_res:
    mastodont_fetch_results_cleanup(&results);
cleanup:
    if (args->params_post) free(post);
    /* Only free if params_query set */
    if (args->params_query) free(url_query);
    return res;
}

