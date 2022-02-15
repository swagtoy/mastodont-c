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

#include <mastodont_application.h>

int mastodont_register_app(mastodont_t* data,
                           struct mstdnt_app_register_args* args,
                           struct mstdnt_storage* storage)
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

    if (mastodont_fetch_curl(data, "api/v1/apps", &results) != CURLE_OK)
        return 1;

    res = mstdnt_load_statuses_from_result(statuses, storage, &results, size);

    mastodont_fetch_results_cleanup(&results);
    
    return res;
}

