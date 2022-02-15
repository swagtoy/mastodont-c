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
#include <mastodont_status.h>
#include <mastodont_account.h>

int mstdnt_load_status_from_json(struct mstdnt_status* status, cJSON* js)
{
    cJSON* v;
    struct _mstdnt_str_val strings[] = {
        { "id", &(status->id) },
        { "uri", &(status->uri) },
        { "created_at", &(status->created_at) },
        { "content", &(status->content) },
        { "spoiler_text", &(status->spoiler_text) },
        { "in_reply_to_id", &(status->in_reply_to_id) },
        { "language", &(status->language) },
        { "url", &(status->url) },
        { "text", &(status->text) },
        { "in_reply_to_account_id", &(status->in_reply_to_account_id) }
    };

    struct _mstdnt_bool_val bools[] = {
        { "sensitive", &(status->sensitive) },
        { "favourited", &(status->favourited) },
        { "reblogged", &(status->reblogged) },
        { "muted", &(status->muted) },
        { "bookmarked", &(status->bookmarked) },
        { "pinned", &(status->pinned) }
    };
    
    for (v = js; v; v = v->next)
    {
        if (_mstdnt_key_val_iter(v, strings, _mstdnt_arr_len(strings),
                                 bools, _mstdnt_arr_len(bools)) == 1)
        {
            if (cJSON_IsObject(v))
            {
                if (strcmp("account", v->string) == 0)
                    mstdnt_load_account_from_json(&(status->account), v->child);
            }
        }
    }
}

int mstdnt_load_statuses_from_result(struct mstdnt_status* statuses[],
                                     struct mstdnt_storage* storage,
                                     struct mstdnt_fetch_results* results,
                                     size_t* size)
{
    size_t i = 0;
    cJSON* root, *status_j_list;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    if (!cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    /* malloc array - cJSON does a loop to count, let's do it once preferably */
    *statuses = malloc((size ? *size : cJSON_GetArraySize(root))
                       * sizeof(struct mstdnt_status));
    if (*statuses == NULL)
        return 1;
    
    cJSON_ArrayForEach(status_j_list, root)
    {
        mstdnt_load_status_from_json((*statuses) + i++, status_j_list->child);
    }
}

int mastodont_account_statuses(mastodont_t* data,
                               char* id,
                               struct mstdnt_account_statuses_args* args,
                               struct mstdnt_storage* storage,
                               struct mstdnt_status* statuses[],
                               size_t* size)
{
    int res;
    char url[MSTDNT_URLSIZE];
    struct mstdnt_fetch_results results = { 0 };
    snprintf(url, MSTDNT_URLSIZE, "api/v1/accounts/%s/statuses", id);
    
    /* Default args */
    struct mstdnt_account_statuses_args _args;
    if (args == NULL)
    {
        _args.pinned = 0;
        _args.tagged = NULL;
        _args.with_muted = 1;
        _args.offset = 0;
        _args.exclude_reblogs = 0;
        _args.exclude_replies = 0;
        _args.only_media = 0;
        _args.max_id = NULL;
        _args.since_id = NULL;
        _args.min_id = NULL;
        _args.limit = 20;
        args = &_args;
    }
    storage->needs_cleanup = 0;

    if (mastodont_fetch_curl(data, url, &results) != CURLE_OK)
        return 1;

    res = mstdnt_load_statuses_from_result(statuses, storage, &results, size);

    mastodont_fetch_results_cleanup(&results);
    
    return res;
}
