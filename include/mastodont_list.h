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

struct mstdnt_list
{
    char* id;
    char* title;
    enum mstdnt_list_replies_policy replies_policy;
};

struct mstdnt_list_args
{
    char* title;
    enum mstdnt_list_replies_policy replies_policy;
};

int mstdnt_list_json(struct mstdnt_list* list, cJSON* js);

int mstdnt_lists_json(struct mstdnt_list* lists[],
                      size_t* size,
                      cJSON* json);

int mstdnt_get_lists(mstdnt_t* api,
                        struct mstdnt_args* m_args,
                        struct mstdnt_storage* storage,
                        struct mstdnt_list* lists[],
                        size_t* size);

int mstdnt_get_list(mstdnt_t* api,
                       struct mstdnt_args* m_args,
                       char* id,
                       struct mstdnt_storage* storage,
                       struct mstdnt_list* lists);

int mstdnt_create_list(mstdnt_t* api,
                          struct mstdnt_args* m_args,
                          struct mstdnt_list_args* args,
                          struct mstdnt_storage* storage,
                          struct mstdnt_list* list);

int mstdnt_update_list(mstdnt_t* api,
                          struct mstdnt_args* m_args,
                          char* id,
                          struct mstdnt_list_args* args,
                          struct mstdnt_storage* storage,
                          struct mstdnt_list* list);

int mstdnt_delete_list(mstdnt_t* api,
                          struct mstdnt_args* m_args,
                          char* id,
                          struct mstdnt_storage* storage);

int mstdnt_list_get_accounts(mstdnt_t* data,
                                struct mstdnt_args* m_args,
                                char* id,
                                struct mstdnt_account_args* args,
                                struct mstdnt_storage* storage,
                                struct mstdnt_account* accts[],
                                size_t* accts_len);

int mstdnt_list_add_accounts(mstdnt_t* api,
                                struct mstdnt_args* m_args,
                                char* id,
                                char** account_ids,
                                size_t account_ids_len,
                                struct mstdnt_storage* storage);

int mstdnt_list_remove_accounts(mstdnt_t* api,
                                   struct mstdnt_args* m_args,
                                   char* id,
                                   char** account_ids,
                                   size_t account_ids_len,
                                   struct mstdnt_storage* storage);

// Cleanup
void mstdnt_cleanup_lists(struct mstdnt_list* lists);

#endif /* MASTODONT_LIST_H */
