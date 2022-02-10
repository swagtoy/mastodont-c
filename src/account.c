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
                      int lookup,
                      char* id,
                      struct mstdnt_account* acct,
                      struct mstdnt_storage* storage,
                      size_t* size)
{
    int res;
    cJSON* root;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE,
             lookup ? "/api/v1/accounts/%s" : "/api/v1/accounts/lookup?acct=%s",
             id);
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results) != CURLE_OK)
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
    struct _mstdnt_str_val strings[] = {
        { "id", &(acct->id) },
        { "username", &(acct->username) },
        { "acct", &(acct->acct) },
        { "display_name", &(acct->display_name) },
        { "created_at", &(acct->created_at) },
        { "note", &(acct->note) },
        { "url", &(acct->url) },
        { "avatar", &(acct->avatar) },
        { "avatar_static", &(acct->avatar_static) },
        { "header", &(acct->header) },
        { "header_static", &(acct->header_static) },
        { "last_status_at", &(acct->last_status_at) },
        { "mute_expires_at", &(acct->mute_expires_at) }
    };

    struct _mstdnt_bool_val bools[] = {
        { "locked", &(acct->locked) },
        { "bot", &(acct->bot) }
    };
    
    for (v = js; v; v = v->next)
    {
        _mstdnt_key_val_iter(v, strings, _mstdnt_arr_len(strings),
                             bools, _mstdnt_arr_len(bools));
    }
}
