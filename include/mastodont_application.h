/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_APPLICATION
#define MASTODONT_APPLICATION
#include "mastodont_types.h"
#include "mastodont_fetch.h"
#include <cjson/cJSON.h>

#ifdef __cplusplus
extern "C" {
#endif

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

struct mstdnt_application_args
{
    char* grant_type;
    char* client_id;
    char* client_secret;
    char* redirect_uri;
    char* scope;
    char* code;
    char* username;
    char* password;
    char* client_name;
    char* redirect_uris;
    char* scopes;
    char* website;
};

int
mstdnt_register_app(mastodont_t* data,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    struct mstdnt_application_args args);

int
mstdnt_obtain_oauth_token(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          mstdnt_request_cb_t cb_request,
                          void* cb_args,
                          struct mstdnt_application_args args);

void _mstdnt_val_malloc_application_call(cJSON* v, void* _type);

#ifdef __cplusplus
}
#endif

#endif /* MASTODONT_ACCOUNT */
