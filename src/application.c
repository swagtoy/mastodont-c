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

static int mstdnt_read_app_result(struct mstdnt_storage* storage,
                                  struct mstdnt_fetch_results* results,
                                  struct mstdnt_app* app)
{
    cJSON* root, *v;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    struct _mstdnt_val_ref refs[] = {
        { "id", &(app->id), _mstdnt_val_string_call },
        { "name", &(app->name), _mstdnt_val_string_call },
        { "website", &(app->website), _mstdnt_val_string_call },
        { "redirect_uri", &(app->redirect_uri), _mstdnt_val_string_call },
        { "client_id", &(app->client_id), _mstdnt_val_string_call },
        { "client_secret", &(app->client_secret), _mstdnt_val_string_call },
        { "vapid_key", &(app->vapid_key), _mstdnt_val_string_call },
    };

    for (v = root->child; v; v = v->next)
    {
        if (_mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs)))
        {
            return 1;
        }
    }
    return 0;
}

static int mstdnt_read_token_result(struct mstdnt_storage* storage,
                                    struct mstdnt_fetch_results* results,
                                    struct mstdnt_oauth_token* app)
{
    cJSON* root, *v;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    struct _mstdnt_val_ref refs[] = {
        { "access_token", &(app->access_token), _mstdnt_val_string_call },
        { "token_type", &(app->token_type), _mstdnt_val_string_call },
        { "scope", &(app->scope), _mstdnt_val_string_call },
        { "id", &(app->id), _mstdnt_val_string_call },
        { "me", &(app->me), _mstdnt_val_string_call },
    };

    for (v = root; v; v = v->next)
    {
        if (_mstdnt_key_val_ref(v->child, refs, _mstdnt_arr_len(refs)) == 1)
        {
            return 1;
        }
    }

    return 0;
}



int mastodont_register_app(mastodont_t* data,
                           struct mstdnt_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_app* app)
{
    int res = 0;
    struct mstdnt_fetch_results results = { 0 };
    
    /* Default args */
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

    if (mastodont_fetch_curl(data, "api/v1/apps", &results, CURLOPT_POST) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }
/*
    if (mstdnt_check_error(&results, storage))
    {
        res = 1;
        goto cleanup_fetch;
    }
*/

    res = mstdnt_read_app_result(storage, &results, app);

cleanup_fetch:
    mastodont_fetch_results_cleanup(&results);
cleanup:
    free(post);
    return res;
}


int mastodont_obtain_oauth_token(mastodont_t* data,
                                 struct mstdnt_args* args,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_oauth_token* token)
{
    int res = 0;
    struct mstdnt_fetch_results results = { 0 };
    
    /* Default args */
    storage->needs_cleanup = 0;

    union param_value u_grant_type, u_client_id,
        u_client_secret, u_redirect_uri,
        u_scope, u_code, u_username, u_password;
    u_grant_type.s = args->grant_type;
    u_client_id.s = args->client_id;
    u_client_secret.s = args->client_secret;
    u_redirect_uri.s = args->redirect_uri;
    u_scope.s = args->scope;
    u_code.s = args->code;
    u_username.s = args->username;
    u_password.s = args->password;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "grant_type", u_grant_type },
        { _MSTDNT_QUERY_STRING, "client_id", u_client_id },
        { _MSTDNT_QUERY_STRING, "client_secret", u_client_secret },
        { _MSTDNT_QUERY_STRING, "redirect_uri", u_redirect_uri },
        { _MSTDNT_QUERY_STRING, "scope", u_scope },
        { _MSTDNT_QUERY_STRING, "code", u_code },
        { _MSTDNT_QUERY_STRING, "username", u_username },
        { _MSTDNT_QUERY_STRING, "password", u_password },        
    };

    char* post = _mstdnt_query_string(NULL, params, _mstdnt_arr_len(params));
    curl_easy_setopt(data->curl, CURLOPT_POSTFIELDS, post);

    if (mastodont_fetch_curl(data, "oauth/token", &results, CURLOPT_POST) != CURLE_OK)
    {
        res = 1;
        goto cleanup;
    }

    if (mstdnt_check_error(&results, storage))
    {
        res = 1;
        goto cleanup_fetch;
    }

    res = mstdnt_read_token_result(storage, &results, token);

cleanup_fetch:
    mastodont_fetch_results_cleanup(&results);

cleanup:
    free(post);
    return res;    
}
