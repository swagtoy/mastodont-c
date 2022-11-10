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

#ifndef MASTODONT_TYPES_H
#define MASTODONT_TYPES_H
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdint.h>

#define _mstdnt_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))
#define MSTDNT_URLSIZE 2048
#define MSTDNT_URISIZE 512
typedef int8_t mstdnt_bool;
// Don't be fooled!
#define MSTDNT_TRUE 2
#define MSTDNT_FALSE 1
#define MSTDNT_BOOL_UNSET 0

struct mstdnt_storage;

typedef void (*mstdnt_data_free_cb_t)(void*, size_t);

typedef struct mstdnt_request_cb_data {
    struct mstdnt_storage* storage;
    void* data;
    mstdnt_data_free_cb_t data_free_cb;
} mstdnt_request_cb_data;

typedef void (*mstdnt_request_cb_t)(mstdnt_request_cb_data* data, void* args);

// Return types for mstdnt_request_cb_t, specifies
//  what we want to do with the data
typedef enum mstdnt_enum_req
{
    MSTDNT_REQUEST_DONE = 0,
    MSTDNT_REQUEST_DATA_NOCLEANUP = 1,
} mstdnt_enum_req;

#define MSTDNT_CB_DATA(_data) (_data->data)
#define MSTDNT_FLAG_NO_URI_SANITIZE (1<<0)
#define MSTDNT_FLAG_SSL_UNVERIFIED (1<<1)
#define MSTDNT_FLAG_SYNC (1<<2)
#define MSTDNT_FLAG_ISSET(flags, flag) (((flags) & (flag)) == (flag))
#define MSTDNT_T_FLAG_ISSET(flag_ref, flag) (((flag_ref->flags) & (flag)) == (flag))

typedef struct mstdnt
{
    CURLM* curl;
} mastodont_t;

struct mstdnt_storage
{
    int needs_cleanup; /* Double free safe */
    cJSON* root;
    char* error;
    char* error_description;
};

struct mstdnt_args
{
    const char* url;
    const char* token;
    uint8_t flags;
};

struct mstdnt_file
{
    char* file;
    char* filename;
    size_t filesize;
    char* filetype;
};

#endif /* MASTODONT_TYPES_H */
