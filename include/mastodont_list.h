/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_LIST_H
#define MASTODONT_LIST_H
#include "mastodont_types.h"
#include <cjson/cJSON.h>
#include "mastodont_fetch.h"
#include "mastodont_account.h"

enum mstdnt_list_replies_policy
{
    MSTDNT_LIST_REPLIES_POLICY_NONE,
    MSTDNT_LIST_REPLIES_POLICY_LIST,
    // Not sure why there is one for followed... you cannot add
    // people to a list who don't follow you? I'd imagine it's the same as
    // the last one.
    MSTDNT_LIST_REPLIES_POLICY_FOLLOWED
};

typedef struct mstdnt_list
{
    char* id;
    char* title;
    enum mstdnt_list_replies_policy replies_policy;
} mstdnt_list;

typedef struct mstdnt_lists
{
    struct mstdnt_list* lists;
    size_t len;
} mstdnt_lists;

struct mstdnt_list_args
{
    char* title;
    enum mstdnt_list_replies_policy replies_policy;
};

int
mstdnt_list_json(struct mstdnt_list* list,
                 cJSON* js);

int
mstdnt_lists_json(struct mstdnt_list* lists[],
                  size_t* size,
                  cJSON* json);

int
mstdnt_get_lists(mastodont_t* api,
                 struct mstdnt_args* m_args,
                 mstdnt_request_cb_t cb_request,
                 void* cb_args);

int
mstdnt_get_list(mastodont_t* api,
                struct mstdnt_args* m_args,
                mstdnt_request_cb_t cb_request,
                void* cb_args,
                char* id);

int
mstdnt_create_list(mastodont_t* api,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   struct mstdnt_list_args args);

int
mstdnt_update_list(mastodont_t* api,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   char* id,
                   struct mstdnt_list_args args);

int
mstdnt_delete_list(mastodont_t* api,
                   struct mstdnt_args* m_args,
                   mstdnt_request_cb_t cb_request,
                   void* cb_args,
                   char* id);

int
mstdnt_list_get_accounts(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* id,
                         struct mstdnt_account_args args);

int
mstdnt_list_add_accounts(mastodont_t* api,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char* id,
                         char** account_ids,
                         size_t account_ids_len);

int
mstdnt_list_remove_accounts(mastodont_t* api,
                            struct mstdnt_args* m_args,
                            mstdnt_request_cb_t cb_request,
                            void* cb_args,
                            char* id,
                            char** account_ids,
                            size_t account_ids_len);

// Cleanup
void mstdnt_cleanup_lists(struct mstdnt_lists* lists);

#endif /* MASTODONT_LIST_H */
