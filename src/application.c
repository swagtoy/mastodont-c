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
#include <mastodont_json_helper.h>
#include <mastodont_application.h>
#include <mastodont_query.h>

int mstdnt_load_app_result(struct mstdnt_storage* storage,
                           struct mstdnt_fetch_results* results,
                           struct mstdnt_app* app)
{
    cJSON* root, *v;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    struct _mstdnt_str_val strings[] = {
        { "id", &(app->id) },
        { "name", &(app->name) },
        { "website", &(app->website) },
        { "redirect_uri", &(app->redirect_uri) },
        { "client_id", &(app->client_id) },
        { "client_secret", &(app->client_secret) },
        { "vapid_key", &(app->vapid_key) },
    };

    for (v = root->child; v; v = v->next)
    {
        if (_mstdnt_key_val_iter(v, strings, _mstdnt_arr_len(strings),
                                 NULL, 0) == 1)
        {
            return 1;
        }
    }
}


int mastodont_register_app(mastodont_t* data,
                           struct mstdnt_app_register_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_app* app)
{
    int res;
    struct mstdnt_fetch_results results = { 0 };
    
    /* Default args */
    struct mstdnt_app_register_args _args;
    if (args == NULL)
    {
        _args.client_name = "mastodont-c"; /* Defaults to false */
        _args.redirect_uris = NULL;
        _args.scopes = NULL;
        _args.website = NULL;
        args = &_args;
    }
    storage->needs_cleanup = 0;

    union param_value u_client_name, u_redirect_uris,
        u_scopes, u_website;
    u_client_name.s = args->client_name;
    u_redirect_uris.s = args->redirect_uris;
    u_scopes.s = args->scopes;
    u_website.s = args->website;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "client_name", u_client_name },
        { _MSTDNT_QUERY_STRING, "redirect_uris", u_redirect_uris },
        { _MSTDNT_QUERY_STRING, "scopes", u_scopes },
        { _MSTDNT_QUERY_STRING, "website", u_website },
    };

    char* post = _mstdnt_query_string(NULL, params, _mstdnt_arr_len(params));

    curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);

    if (mastodont_fetch_curl(data, "api/v1/apps", &results) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }

    res = mstdnt_load_app_result(storage, &results, app);

    mastodont_fetch_results_cleanup(&results);

cleanup:
    free(post);
    return res;
}

