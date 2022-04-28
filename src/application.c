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

#include <string.h>
#include <stdlib.h>
#include <mastodont_json_helper.h>
#include <mastodont_application.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

int mstdnt_app_result(struct mstdnt_fetch_results* results,
                      struct mstdnt_storage* storage,
                      struct mstdnt_app* app)
{
    cJSON* root, *v;

    /* Zero out */
    memset(storage, 0, sizeof(struct mstdnt_storage));
    memset(app, 0, sizeof(struct mstdnt_app));
    
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

static int mstdnt_app_result_callback(struct mstdnt_fetch_results* results,
                                   struct mstdnt_storage* storage,
                                   void* args)
{
    return mstdnt_app_result(results, storage, args);
}

int mstdnt_token_result(struct mstdnt_fetch_results* results,
                               struct mstdnt_storage* storage,
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

static int mstdnt_token_result_callback(struct mstdnt_fetch_results* results,
                                        struct mstdnt_storage* storage,
                                        void* args)
{
    return mstdnt_token_result(results, storage, args);
}

int mastodont_register_app(mastodont_t* data,
                           struct mstdnt_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_app* app)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "client_name", { .s = args->client_name } },
        { _MSTDNT_QUERY_STRING, "redirect_uris", { .s = args->redirect_uris } },
        { _MSTDNT_QUERY_STRING, "scopes", { .s = args->scopes } },
        { _MSTDNT_QUERY_STRING, "website", { .s = args->website } },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/apps",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        app,
        mstdnt_app_result_callback
    };

    return mastodont_request(data, &req_args);
}


int mastodont_obtain_oauth_token(mastodont_t* data,
                                 struct mstdnt_args* args,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_oauth_token* token)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "grant_type", { .s = args->grant_type } },
        { _MSTDNT_QUERY_STRING, "client_id", { .s = args->client_id } },
        { _MSTDNT_QUERY_STRING, "client_secret", { .s = args->client_secret } },
        { _MSTDNT_QUERY_STRING, "redirect_uri", { .s = args->redirect_uri } },
        { _MSTDNT_QUERY_STRING, "scope", { .s = args->scope } },
        { _MSTDNT_QUERY_STRING, "code", { .s = args->code } },
        { _MSTDNT_QUERY_STRING, "username", { .s = args->username } },
        { _MSTDNT_QUERY_STRING, "password", { .s = args->password } },
    };

    struct mastodont_request_args req_args = {
        storage,
        "oauth/token",
        NULL,
        0,
        params,
        _mstdnt_arr_len(params),
        CURLOPT_POST,
        token,
        mstdnt_token_result_callback
    };

    return mastodont_request(data, &req_args);
}
