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

#ifndef MASTODONT_APPLICATION
#define MASTODONT_APPLICATION
#include "mastodont_types.h"
#include <cjson/cJSON.h>
#include <mastodont_fetch.h>

struct mstdnt_application
{
    char* name;
    char* website;
    char* vapid_key;
};

struct mstdnt_app_register_args
{
    char* client_name;
    char* redirect_uris;
    char* scopes;
    char* website;
};

struct mstdnt_app
{
    char* id;
    char* name;
    char* website;
    char* redirect_uri;
    char* client_id;
    char* client_secret;
    char* vapid_key;
};

struct mstdnt_oauth_token
{
    char* access_token;
    char* token_type;
    char* scope;
    char* id;
    char* me;
    time_t time;
};

struct mstdnt_oauth_token_args
{
    char* grant_type;
    char* client_id;
    char* client_secret;
    char* redirect_uri;
    char* scope;
    char* code;
    char* username;
    char* password;
};

int mastodont_register_app(mastodont_t* data,
                           struct mstdnt_app_register_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_app* app);

int mastodont_obtain_oauth_token(mastodont_t* data,
                                 struct mstdnt_oauth_token_args* args,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_oauth_token* app);

#endif /* MASTODONT_ACCOUNT */
