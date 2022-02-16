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
#include <string.h>
#include "mastodont_fetch.h"

/* For use with libcurl */
static size_t write_callback(char* ptr, size_t _size, size_t nmemb, void* _content)
{
    size_t size = nmemb * _size; /* Mostly pointless, but portable */
    struct mstdnt_fetch_results* res = _content; /* Cast */
    char* data;
 
    if ((data = realloc(res->response, res->size + size + 1)) == NULL)
    {
        perror("realloc");
        return 0;
    }
 
    res->response = data;
    memcpy(&(res->response[res->size]), ptr, size);
    res->size += size;
    res->response[res->size] = 0;
 
    return size;
}

void mastodont_fetch_results_cleanup(struct mstdnt_fetch_results* res)
{
    free(res->response);
}

#include <stdio.h>
#define TOKEN_STR_SIZE 512
int mastodont_fetch_curl(mastodont_t* mstdnt,
                         char* _url,
                         struct mstdnt_fetch_results* results)
{
    int res;
    char token[TOKEN_STR_SIZE] = { 0 };
    struct curl_slist* list = NULL;

    /* Setup URL */
    char url[MSTDNT_URLSIZE];
    strncpy(url, mstdnt->url, MSTDNT_URLSIZE-1);
    strncat(url, _url, MSTDNT_URLSIZE-1);

    /* Setup token */
    if (mstdnt->token)
    {
        snprintf(token, TOKEN_STR_SIZE, "Authorization: Bearer %s",
                 mstdnt->token);
        list = curl_slist_append(list, token);
        curl_easy_setopt(mstdnt->curl, CURLOPT_HTTPHEADER, list);
    }

    /* Set options */
    curl_easy_setopt(mstdnt->curl, CURLOPT_URL, url);
    curl_easy_setopt(mstdnt->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(mstdnt->curl, CURLOPT_WRITEDATA, results);

    res = curl_easy_perform(mstdnt->curl);

    if (list) curl_slist_free_all(list);

    return res;
}
