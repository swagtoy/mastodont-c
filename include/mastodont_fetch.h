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

#ifndef MASTODONT_FETCH_H
#define MASTODONT_FETCH_H
#include <cjson/cJSON.h>
#include <mastodont_types.h>
#include <curl/curl.h>

struct mstdnt_fetch_results
{
    char* response;
    size_t size;
};

size_t mstdnt_curl_write_callback(char* ptr, size_t _size, size_t nmemb, void* _content);
void mastodont_fetch_results_cleanup(struct mstdnt_fetch_results* res);
int mastodont_fetch_curl(mastodont_t* mstdnt,
                         char* url,
                         struct mstdnt_fetch_results* results,
                         CURLoption request_t,
                         char* request_t_custom);

#endif /* MASTODONT_FETCH_H */
