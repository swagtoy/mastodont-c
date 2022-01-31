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
