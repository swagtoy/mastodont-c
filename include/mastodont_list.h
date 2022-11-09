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

#ifndef MASTODONT_LIST_H
#define MASTODONT_LIST_H
#include <mastodont_types.h>
#include <cjson/cJSON.h>
#include <mastodont_fetch.h>
#include <mastodont_account.h>

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
