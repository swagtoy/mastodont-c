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
#include <mastodont_hooks.h>
#include <mastodont_fetch.h>
#include <mastodont_json_helper.h>

/* For use with libcurl */
size_t mstdnt_curl_write_callback(char* ptr, size_t _size, size_t nmemb, void* _content)
{
    size_t size = nmemb * _size; /* Mostly pointless, but portable */
    struct mstdnt_fetch_data* res = _content; /* Cast */
    char* data;
 
    if ((data = mstdnt_realloc(res->response, res->size + size + 1)) == NULL)
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

void mstdnt_fetch_data_cleanup(struct mstdnt_fetch_data* res)
{
    mstdnt_free(res->response);
}

#define TOKEN_STR_SIZE 512
int mstdnt_fetch_curl_async(mastodont_t* mstdnt,
                            CURL* curl,
                            struct mstdnt_args* m_args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            int (*json_cb)(cJSON*, void*),
                            void* json_args,
                            char* _url,
                            CURLoption request_t,
                            char* request_t_custom)
{
#define is_custom request_t_custom && request_t == CURLOPT_CUSTOMREQUEST
    struct mstdnt_fetch_data* results = NULL;
    CURLMcode res = 0;
    char token[TOKEN_STR_SIZE] = { 0 };
    struct curl_slist* list = NULL;

    // Setup URL
    char url[MSTDNT_URLSIZE] = { 0 };
    strncpy(url, m_args->url, MSTDNT_URLSIZE-1);
    strncat(url, _url, MSTDNT_URLSIZE-1);

    // Setup token
    if (m_args->token)
    {
        snprintf(token, TOKEN_STR_SIZE, "Authorization: Bearer %s",
                 m_args->token);
        list = curl_slist_append(list, token);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }

    // Setup data to pass into results
    results = calloc(1, sizeof(struct mstdnt_fetch_data));
    if (!results)
    {
        perror("calloc");
        return -1;
    }
    results->callback = cb_request;
    results->callback_args = cb_args;

    results->json_cb = json_cb;
    results->json_args = json_args;

    // Set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, mstdnt_curl_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, results);
    /* Copy into private pointer value
     * A little stupid, but we can let CURL hold our values for us.
     * Curl won't let us get the WRITEDATA opt pointer back sadly, so this has to be done */
    curl_easy_setopt(curl, CURLOPT_PRIVATE, results);
    /* Should we verify the peer's SSL cert? */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,
                     !MSTDNT_T_FLAG_ISSET(m_args, MSTDNT_FLAG_SSL_UNVERIFIED));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,
                     !MSTDNT_T_FLAG_ISSET(m_args, MSTDNT_FLAG_SSL_UNVERIFIED));
    // PUT, POST, GET, Custom
    // Mimes are expected to be set beforehand manually
    if (is_custom)
        curl_easy_setopt(curl, request_t, request_t_custom);
    else if (request_t != CURLOPT_MIMEPOST)
        curl_easy_setopt(curl, request_t, 1);

    // Add curl handle to multi, then run
    res = curl_multi_add_handle(mstdnt->curl, curl);
    if (res != CURLM_OK)
    {
        printf("error %s\n", curl_multi_strerror(res));
        return -1;
    }

    /* int running; */
    /* res = curl_multi_perform(mstdnt->curl, &running); */
    /* if (res != CURLM_OK) */
    /* { */
    /*     printf("error %s\n", curl_multi_strerror(res)); */
    /*     return -1; */
    /* } */

    return 0;
}

int mstdnt_await(mastodont_t* mstdnt,
                 enum mstdnt_fetch_await opt,
                 struct mstdnt_fd extra_fds[],
                 size_t nfds)
{
    CURLMsg* msg;
    int msgs_left = 1;
    int res;
    struct curl_waitfd* fds = NULL;

    // Any other fds can go here
    if (extra_fds)
    {
        fds = calloc(nfds, sizeof(struct curl_waitfd));
        for (int i = 0; i < nfds; ++i)
        {
            fds[i].fd = extra_fds[i].fd;
            fds[i].events = extra_fds[i].events;
            fds[i].revents = extra_fds[i].revents;
        }
    }
    
    struct mstdnt_fetch_data* data;
    cJSON* root;
    struct mstdnt_storage storage = { 0 };
    mstdnt_request_cb_data results = { 0 };
    int numfds;
    int running = 1;

    // Check if our socket is done
    do
    {
        res = curl_multi_perform(mstdnt->curl, &running);
        
        res = curl_multi_poll(mstdnt->curl, fds, nfds, 1000, &numfds);
        
        while ((msg = curl_multi_info_read(mstdnt->curl, &msgs_left)) != NULL)
        {
            if (msg->msg == CURLMSG_DONE)
            {
                // Get easy info
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &data);
                // Setup
                storage.needs_cleanup = 0;
                // Fill in results
                
                // Get json
                if (_mstdnt_json_init(&root, data, &storage))
                {
                    res = 1;
                    goto cleanup_res;
                }

                // Setup callback results
                results.storage = &storage;

                // Yeah, it's like that sometimes... :')
                if (data->json_cb)
                    res = data->json_cb(storage.root, data->json_args, &results);
                
                data->callback(&(results), data->callback_args);

            cleanup_res:
                mstdnt_request_cb_cleanup(&results);
                // Cleanup
                mstdnt_fetch_data_cleanup(data);
                curl_multi_remove_handle(mstdnt->curl, msg->easy_handle);
                curl_easy_cleanup(msg->easy_handle);
            }
        }

        if (res) break;
    }
    while (/* opt == MSTDNT_AWAIT_ALL && msgs_left */ running);

    // Put revents back for callee
    if (extra_fds)
        for (int i = 0; i < nfds; ++i)
            extra_fds[i].revents = fds[i].revents;

    free(fds);

    return res;
}

void mstdnt_storage_cleanup(struct mstdnt_storage* storage);

void mstdnt_request_cb_cleanup(mstdnt_request_cb_data* data)
{
    mstdnt_storage_cleanup(data->storage);
    //data->data_free_cb(data->data, 0);
    /* free(data); */
}
