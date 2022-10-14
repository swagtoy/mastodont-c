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
#include <pthread.h>
#include "mastodont_fetch.h"

/* For use with libcurl */
size_t mstdnt_curl_write_callback(char* ptr, size_t _size, size_t nmemb, void* _content)
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

void mstdnt_fetch_results_cleanup(struct mstdnt_fetch_results* res)
{
    free(res->response);
}

#define TOKEN_STR_SIZE 512
int mstdnt_fetch_curl(mstdnt_t* mstdnt,
                         CURL* curl,
                         struct mstdnt_args* m_args,
                         char* _url,
                         struct mstdnt_fetch_results* results,
                         CURLoption request_t,
                         char* request_t_custom)
{
#define is_custom request_t_custom && request_t == CURLOPT_CUSTOMREQUEST
    CURLMcode res = 3;
    int status = 0;
    CURLMsg* msg;
    int running = 1;
    char token[TOKEN_STR_SIZE] = { 0 };
    struct curl_slist* list = NULL;

    /* Setup URL */
    char url[MSTDNT_URLSIZE] = { 0 };
    strncpy(url, m_args->url, MSTDNT_URLSIZE-1);
    strncat(url, _url, MSTDNT_URLSIZE-1);

    /* Setup token */
    if (m_args->token)
    {
        snprintf(token, TOKEN_STR_SIZE, "Authorization: Bearer %s",
                 m_args->token);
        list = curl_slist_append(list, token);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }

    /* Set options */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mstdnt_curl_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, results);
    /* Should we verify the peer's SSL cert? */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,
                     !MSTDNT_T_FLAG_ISSET(m_args, MSTDNT_FLAG_SSL_UNVERIFIED));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,
                     !MSTDNT_T_FLAG_ISSET(m_args, MSTDNT_FLAG_SSL_UNVERIFIED));
    /* PUT, POST, GET, Custom */
    /* Mimes are expected to be set beforehand manually */
    if (is_custom)
        curl_easy_setopt(curl, request_t, request_t_custom);
    else if (request_t != CURLOPT_MIMEPOST)
        curl_easy_setopt(curl, request_t, 1);

    // Add curl handle to multi, then run and block
#ifndef THREAD_UNSAFE
    static pthread_mutex_t multi_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifndef THREAD_UNSAFE
    pthread_mutex_lock(&multi_mutex);
#endif
    curl_multi_add_handle(mstdnt->curl, curl);
#ifndef THREAD_UNSAFE
    pthread_mutex_unlock(&multi_mutex);
#endif

    int msgs_left;
    while (running)
    {
#ifndef THREAD_UNSAFE
        pthread_mutex_lock(&multi_mutex);
#endif
        res = curl_multi_perform(mstdnt->curl, &running);
#ifndef THREAD_UNSAFE
        pthread_mutex_unlock(&multi_mutex);
#endif

        if (running)
            res = curl_multi_poll(mstdnt->curl, NULL, 0, 1000, NULL);

        // Check if our socket is done
#ifndef THREAD_UNSAEF
        pthread_mutex_lock(&multi_mutex);
#endif
        while ((msg = curl_multi_info_read(mstdnt->curl, &msgs_left)))
        {
            if (msg->msg == CURLMSG_DONE && msg->easy_handle == curl)
            {
                status = msg->data.result;
#ifndef THREAD_UNSAFE
                pthread_mutex_unlock(&multi_mutex);
#endif
                goto out;
            }
        }
#ifndef THREAD_UNSAFE
        pthread_mutex_unlock(&multi_mutex);
#endif

        if (res) break;
    }
    
out:
#ifndef THREAD_UNSAFE
    pthread_mutex_lock(&multi_mutex);
#endif
    // Looks like we're done here
    curl_multi_remove_handle(mstdnt->curl, curl);
    #ifndef THREAD_UNSAFE
    pthread_mutex_unlock(&multi_mutex);
#endif
    
    if (list) curl_slist_free_all(list);
    return status;
}
