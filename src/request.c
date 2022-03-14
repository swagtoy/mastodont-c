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

    storage->needs_cleanup = 0;

    if (args->params_post)
    {
        post = _mstdnt_query_string(data, NULL, args->params_post, args->params_post_len);
        curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);
    }

    if (mastodont_fetch_curl(data, args->url, &results, args->request_type) != CURLE_OK)
    {
        return 1;
    }

    if (mstdnt_check_error(&results, storage))
    {
        res = 1;
        goto cleanup;
    }
    
    args->callback(&results, storage, args->args);

cleanup:
    mastodont_fetch_results_cleanup(&results);
    if (args->params_post) free(post);
    return res;
}

