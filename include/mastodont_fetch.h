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

/** Used to store the response from CURL */
struct mstdnt_fetch_data
{
    char* response;
    size_t size;

    // Callback from user
    mstdnt_request_cb_t callback;
    void* callback_args;
};

enum mstdnt_fetch_await
{
    MSTDNT_AWAIT_ALL,
    MSTDNT_AWAIT_ONCE,
};

size_t mstdnt_curl_write_callback(char* ptr, size_t _size, size_t nmemb, void* _content);
void mstdnt_fetch_data_cleanup(struct mstdnt_fetch_data* res);

/**
 * @brief Attempts to fetch, async or blocking.
 *
 * @param mstdnt Mastodont struct
 * @param curl curl API
 * @param args Mastodont General args passed
 * @param url URL of request
 * @param results Results from response
 */
int mstdnt_fetch_curl_async(mastodont_t* mstdnt,
                            CURL* curl,
                            struct mstdnt_args* args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            char* url,
                            CURLoption request_t,
                            char* request_t_custom);

#endif /* MASTODONT_FETCH_H */
