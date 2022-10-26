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
#include <mastodont_hooks.h>
#include <mastodont_json_helper.h>
#include <mastodont_application.h>
#include <mastodont_query.h>
#include <mastodont_request.h>

static int mstdnt_app_json(cJSON* json, struct mstdnt_app* app)
{
    if (!json) return 1;
    /* Zero out */
    memset(app, 0, sizeof(struct mstdnt_app));
    
    struct _mstdnt_val_ref refs[] = {
        { "id", &(app->id), _mstdnt_val_string_call },
        { "name", &(app->name), _mstdnt_val_string_call },
        { "website", &(app->website), _mstdnt_val_string_call },
        { "redirect_uri", &(app->redirect_uri), _mstdnt_val_string_call },
        { "client_id", &(app->client_id), _mstdnt_val_string_call },
        { "client_secret", &(app->client_secret), _mstdnt_val_string_call },
        { "vapid_key", &(app->vapid_key), _mstdnt_val_string_call },
    };

    for (cJSON* v = json; v; v = v->next)
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));

    return 0;
}

static int mstdnt_app_json_callback(cJSON* json, void* args)
{
    return mstdnt_app_json(json->child, args);
}

static int mstdnt_token_json(cJSON* json, struct mstdnt_oauth_token* app)
{
    struct _mstdnt_val_ref refs[] = {
        { "access_token", &(app->access_token), _mstdnt_val_string_call },
        { "token_type", &(app->token_type), _mstdnt_val_string_call },
        { "scope", &(app->scope), _mstdnt_val_string_call },
        { "id", &(app->id), _mstdnt_val_string_call },
        { "me", &(app->me), _mstdnt_val_string_call },
    };

    for (cJSON* v = json; v; v = v->next)
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    
    return 0;
}

static int mstdnt_token_json_callback(cJSON* json, void* args)
{
    return mstdnt_token_json(json->child, args);
}

void _mstdnt_val_malloc_application_call(cJSON* v, void* _type)
{
    struct mstdnt_app** type = _type;
    // We can skip an array size check by just seeing
    // if the first value of the child is not zero
    if (!(v->child && !cJSON_IsInvalid(v->child)))
    {
        *type = NULL;
        return;
    }


    *type = mstdnt_calloc(1, sizeof(struct mstdnt_app));

    if (*type)
        mstdnt_app_json(v->child, *type);
}

int mstdnt_register_app(mastodont_t* data,
                           struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                           struct mstdnt_application_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_app* app)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "client_name", { .s = args->client_name } },
        { _MSTDNT_QUERY_STRING, "redirect_uris", { .s = args->redirect_uris } },
        { _MSTDNT_QUERY_STRING, "scopes", { .s = args->scopes } },
        { _MSTDNT_QUERY_STRING, "website", { .s = args->website } },
    };
    
    struct mstdnt_request_args req_args = {
        storage,
        "api/v1/apps",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        app,
        mstdnt_app_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

int mstdnt_obtain_oauth_token(mastodont_t* data,
                                 struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                                 struct mstdnt_application_args* args,
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

    struct mstdnt_request_args req_args = {
        storage,
        "oauth/token",
        NULL,
        0,
        params,
        _mstdnt_arr_len(params),
        CURLOPT_POST,
        NULL,
        token,
        mstdnt_token_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

