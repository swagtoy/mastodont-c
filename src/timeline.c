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
#include "mastodont_timeline.h"

int mastodont_timeline_public(mastodont_t* data,
                              struct mstdnt_timeline_public_args* args,
                              struct mstdnt_response* response)
{
    int res;
    char url[MSTDNT_URLSIZE];
    strncpy(url, data->url, MSTDNT_URLSIZE-1);
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

    /* Copy string */
    strncat(url, "/api/v1/timelines/public", MSTDNT_URLSIZE-1);
    curl_easy_setopt(data->curl, CURLOPT_URL, url);
    
    res = curl_easy_perform(data->curl);

    curl_easy_cleanup(data->curl);
    return res;
}
