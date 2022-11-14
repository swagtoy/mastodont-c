/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_ACCOUNT
#define MASTODONT_ACCOUNT
#include "mastodont_types.h"
#include "mastodont_fetch.h"
#include "mastodont_relationship.h"
#include "mastodont_emoji.h"
#include <cjson/cJSON.h>

#define MSTDNT_LOOKUP_ACCT 0
#define MSTDNT_LOOKUP_ID 1

// DEPRECATED
struct _mstdnt_accounts_args
{
    struct mstdnt_account** acct;
    size_t* size;
};

typedef struct mstdnt_accounts
{
    struct mstdnt_account* accts;
    size_t len;
} mstdnt_accounts;

typedef struct mstdnt_account
{
    char* id;
    char* username;
    char* acct;
    char* url;

    /* Display attributes */
    char* display_name;
    char* note;
    char* avatar;
    char* avatar_static;
    char* header;
    char* header_static;
    mstdnt_bool locked;
    mstdnt_bool discoverable;

    struct mstdnt_emoji* emojis;
    size_t emojis_len;

    /* Statistic attributes */
    time_t created_at;
    char* last_status_at;
    unsigned statuses_count;
    unsigned followers_count;
    unsigned following_count;

    /* Optional attributes */
    struct mstdnt_account* moved;
    /* struct mstdnt_field* field; */
    mstdnt_bool bot;
    /* struct mstdnt_source* source */
    mstdnt_bool suspended;
    char* mute_expires_at;
} mstdnt_account;

struct mstdnt_account_args
{
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
    int with_relationships; // mstdnt_get_mutes
};

int
mstdnt_account_action(mastodont_t* data,
                      struct mstdnt_args* args,
                      mstdnt_request_cb_t cb_request,
                      void* cb_args,
                      char* id,
                      char* url_str);

int
mstdnt_accounts_json(struct mstdnt_account* accounts[],
                     size_t* size,
                     cJSON* js);

#define MSTDNT_ACCOUNT_ACTION_DECL(type)                        \
    int mstdnt_##type##_account(mastodont_t* data,              \
                                struct mstdnt_args* args,       \
                                mstdnt_request_cb_t cb_request, \
                                void* cb_args,                  \
                                char* id)
        
#define MSTDNT_ACCOUNT_ACTION_FUNC_URL(action) {                    \
        return mstdnt_account_action(data,                          \
                                     args,                          \
                                     cb_request,                    \
                                     cb_args,                       \
                                     id,                            \
                                     "api/v1/accounts/%s/" action); \
    }

MSTDNT_ACCOUNT_ACTION_DECL(follow);
MSTDNT_ACCOUNT_ACTION_DECL(unfollow);
MSTDNT_ACCOUNT_ACTION_DECL(mute);
MSTDNT_ACCOUNT_ACTION_DECL(unmute);
MSTDNT_ACCOUNT_ACTION_DECL(block);
MSTDNT_ACCOUNT_ACTION_DECL(unblock);
MSTDNT_ACCOUNT_ACTION_DECL(subscribe);
MSTDNT_ACCOUNT_ACTION_DECL(unsubscribe);

int
mstdnt_verify_credentials(mastodont_t* data,
                          struct mstdnt_args* m_args,
                          mstdnt_request_cb_t cb_request,
                          void* cb_args);

int
mstdnt_get_account(mastodont_t* data,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   int lookup_type,
                   char* id);

int
mstdnt_get_blocks(mastodont_t* data,
                  struct mstdnt_args* m_args,
                  mstdnt_request_cb_t cb_request,
                  void* cb_args,
                  struct mstdnt_account_args args);

int
mstdnt_get_mutes(mastodont_t* data,
                 struct mstdnt_args* m_args,
                 mstdnt_request_cb_t cb_request,
                 void* cb_args,
                 struct mstdnt_account_args args);

int
mstdnt_get_followers(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* id,
                     struct mstdnt_account_args args);

int
mstdnt_get_following(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* id,
                     struct mstdnt_account_args args);

int
mstdnt_account_json(struct mstdnt_account* acct,
                    cJSON* js);

int
mstdnt_account_json_callback(cJSON* json,
                             void* args,
                             mstdnt_request_cb_data* data);
int
mstdnt_accounts_json_callback(cJSON* json,
                             void* args,
                             mstdnt_request_cb_data* data);
void _mstdnt_val_account_call(cJSON* v, void* _type);
void _mstdnt_val_malloc_account_call(cJSON* v, void* _type);

// Cleanup
void mstdnt_cleanup_account(struct mstdnt_account* acct);
void mstdnt_cleanup_accounts(struct mstdnt_accounts* accts);

#endif /* MASTODONT_ACCOUNT */
