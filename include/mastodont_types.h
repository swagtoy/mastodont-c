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
typedef unsigned char mstdnt_bool;

// It's more logical to not sanitize than to sanitize data
#define MSTDNT_FLAG_NO_URI_SANITIZE (1<<0)
#define MSTDNT_FLAG_SSL_UNVERIFIED (1<<1)
#define MSTDNT_FLAG_ISSET(flags, flag) (((flags) & (flag)) == (flag))
#define MSTDNT_T_FLAG_ISSET(flag_ref, flag) (((flag_ref->flags) & (flag)) == (flag))

typedef struct mastodont
{
    CURL* curl;
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
