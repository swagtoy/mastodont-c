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
#include <mastodont_account.h>
#include <mastodont_request.h>
#include <mastodont_json_helper.h>
#include "mastodont_fetch.h"

struct mastodont_account_args
{
    struct mstdnt_account* acct;
    size_t* size;
};

static int mastodont_account_callback(struct mstdnt_fetch_results* results,
                                      struct mstdnt_storage* storage,
                                      void* _args)
{
    struct mastodont_account_args* args = _args;
    struct mstdnt_account* acct = args->acct;
    size_t* size = args->size;
    
    cJSON* root;
    /* TODO cleanup this */
    root = cJSON_ParseWithLength(results->response, results->size);

    if (root == NULL)
    {
        return 1;
    }
    storage->root = root;
    storage->needs_cleanup = 1;

    mstdnt_load_account_from_json(acct, root->child);
    return 0;
}

int mastodont_account(mastodont_t* data,
                      int lookup, /* TODO move into separate function for consistancy */
                      char* id,
                      struct mstdnt_account* acct,
                      struct mstdnt_storage* storage,
                      size_t* size)
{
    struct mastodont_account_args acct_args = { acct, size };
    /* Url */
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE,
             lookup ? "api/v1/accounts/%s" : "api/v1/accounts/lookup?acct=%s",
             id);
    
    struct mastodont_request_args args = {
        storage,
        url,
        NULL,
        0,
        NULL,
        0,
        CURLOPT_HTTPGET,
        &acct_args, /* args */
        mastodont_account_callback, /* callback */
    };
    
    return mastodont_request(data, &args);
}


int mstdnt_load_account_from_json(struct mstdnt_account* acct, cJSON* js)
{
    cJSON* v;
    struct _mstdnt_val_ref refs[] = {
        { "id", &(acct->id), _mstdnt_val_string_call },
        { "username", &(acct->username), _mstdnt_val_string_call },
        { "acct", &(acct->acct), _mstdnt_val_string_call },
        { "display_name", &(acct->display_name), _mstdnt_val_string_call },
        { "created_at", &(acct->created_at), _mstdnt_val_string_call },
        { "note", &(acct->note), _mstdnt_val_string_call },
        { "url", &(acct->url), _mstdnt_val_string_call },
        { "avatar", &(acct->avatar), _mstdnt_val_string_call },
        { "avatar_static", &(acct->avatar_static), _mstdnt_val_string_call },
        { "header", &(acct->header), _mstdnt_val_string_call },
        { "header_static", &(acct->header_static), _mstdnt_val_string_call },
        { "last_status_at", &(acct->last_status_at), _mstdnt_val_string_call },
        { "mute_expires_at", &(acct->mute_expires_at), _mstdnt_val_string_call },
        { "locked", &(acct->locked), _mstdnt_val_bool_call },
        { "bot", &(acct->bot), _mstdnt_val_bool_call },
        { "statuses_count", &(acct->statuses_count), _mstdnt_val_uint_call },
        { "followers_count", &(acct->followers_count), _mstdnt_val_uint_call },
        { "following_count", &(acct->following_count), _mstdnt_val_uint_call },
    };
    
    for (v = js; v; v = v->next)
    {
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    }
}
