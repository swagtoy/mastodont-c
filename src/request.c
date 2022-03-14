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

#include <assert.h>
#include <mastodont_request.h>

int mastodont_request(mastodont_t* data, struct mastodont_request_args* args)
{
    int res = 0;
    struct mstdnt_storage* storage = args->storage;
    struct mstdnt_fetch_results results = { 0 };

    if (mastodont_fetch_curl(data, args->url, &results, CURLOPT_HTTPGET) != CURLE_OK)
    {
        return 1;
    }

    /* TODO Check if error json */

    args->callback(&results, storage, args->args);

cleanup:
    mastodont_fetch_results_cleanup(&results);
    return res;
}

