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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <mastodont_hooks.h>
#include <mastodont_request.h>
#include <mastodont_query.h>
#include <mastodont_error.h>
#include <mastodont_json_helper.h>

#define CONV_SIZE 64

static void mime_params_post(curl_mime* mime,
                             struct _mstdnt_query_param* params,
                             size_t size)
{
    size_t i;
    curl_mimepart* part;
    char conv_val[CONV_SIZE];
    char* val_ptr;
    char* escape_str;
    struct mstdnt_file* file = NULL;

    for (i = 0; i < size; ++i)
    {
        part = curl_mime_addpart(mime);

        /* Skip if file is empty */
        if (params[i].type == _MSTDNT_QUERY_FILE &&
            params[i].value.f == NULL)
            continue;
        
        switch (params[i].type)
        {
        case _MSTDNT_QUERY_INT:
            snprintf(conv_val, CONV_SIZE, "%d", params[i].value.i);
            val_ptr = conv_val;
            break;
        case _MSTDNT_QUERY_STRING:
            val_ptr = params[i].value.s;
            break;
        case _MSTDNT_QUERY_FILE:
            file = params[i].value.f;
            val_ptr = file->file;
            curl_mime_type(part, file->filetype);
            curl_mime_filename(part, file->filename);
        default:
            /* Any other types are not supported! */
            break;
        }

        curl_mime_data(part, val_ptr,
                       file ? file->filesize : CURL_ZERO_TERMINATED);
        curl_mime_name(part, params[i].key);
    }
    
}

int mstdnt_request(mastodont_t* data,
                      struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                      struct mstdnt_request_args* args)
{
    int res = 0, curlerror = 0;
    struct mstdnt_storage* storage = args->storage;
    struct mstdnt_fetch_results results = { 0 };
    cJSON* root;
    curl_mime* mime = NULL;
    char* post;
    // TODO debug me
    char* url_query = args->params_query ?
        _mstdnt_query_string(data, m_args, args->url, args->params_query, args->params_query_len) :
        args->url;

    // Create cURL single handle, we will run this later and then block
    CURL* curl = curl_easy_init();

    /* Zero out */
    memset(storage, 0, sizeof(struct mstdnt_storage));
    storage->needs_cleanup = 0;

    if (args->params_post &&
        (args->request_type == CURLOPT_POST ||
         args->request_type == CURLOPT_CUSTOMREQUEST))
    {
        post = _mstdnt_query_string(data, m_args, NULL, args->params_post, args->params_post_len);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
    }
    else if (args->params_post && args->request_type == CURLOPT_MIMEPOST)
    {
        /* Initialize MIME post */
        mime = curl_mime_init(curl);
        curl_easy_setopt(curl, args->request_type, mime);

        /* Let's handle query params array */
        mime_params_post(mime, args->params_post, args->params_post_len);
    }
    /* Make it empty, no post data provided */
    /* I'm not sure why the pleroma api does this */
    else if (args->request_type == CURLOPT_POST)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

    curlerror = mstdnt_fetch_curl(data,
                                     curl,
                                     m_args,
                                     url_query,
                                     &results,
                                     args->request_type,
                                     args->request_type_custom);

    if (mime) curl_mime_free(mime);

    if (curlerror != CURLE_OK)
    {
        res = 1;
        storage->error = (char*)curl_easy_strerror(curlerror);
        goto cleanup;
    }

    // Create json structure
    if (_mstdnt_json_init(&root, &results, storage))
    {
        res = 1;
        goto cleanup_res;
    }

    // Make sure there is no error
    if (!mstdnt_check_error(storage))
    {
        /* Call our callback and do the large work */
        if (args->callback) res = args->callback(storage->root, args->args);
    }
    else
        res = 1;

cleanup_res:
    mstdnt_fetch_results_cleanup(&results);
cleanup:
    // Note: the fetch removed the handle from our multi handle
    curl_easy_cleanup(curl);
    
    if (args->params_post && args->request_type == CURLOPT_POST) mstdnt_free(post);
    /* Only free if params_query set */
    if (args->params_query) mstdnt_free(url_query);
    return res;
}
