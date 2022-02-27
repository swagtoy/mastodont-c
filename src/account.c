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
#include <mastodont_json_helper.h>
#include "mastodont_fetch.h"

int mastodont_account(mastodont_t* data,
                      int lookup, /* TODO move into separate function for consistancy */
                      char* id,
                      struct mstdnt_account* acct,
                      struct mstdnt_storage* storage,
                      size_t* size)
{
    int res = 0;
    cJSON* root;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE,
             lookup ? "api/v1/accounts/%s" : "api/v1/accounts/lookup?acct=%s",
             id);
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results, CURLOPT_HTTPGET) != CURLE_OK)
    {
        return 1;
    }

    /* TODO cleanup this */
    root = cJSON_ParseWithLength(results.response, results.size);

    if (root == NULL)
    {
        res = 1;
        goto cleanup;
    }
    storage->root = root;
    storage->needs_cleanup = 1;

    mstdnt_load_account_from_json(acct, root->child);
cleanup:
    mastodont_fetch_results_cleanup(&results);

    return res;
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
