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
#include <mastodont_tag.h>
#include <mastodont_history.h>
#include <mastodont_generate.h>
#include <mastodont_json_helper.h>

int mstdnt_tag_json(struct mstdnt_tag* tag, cJSON* js)
{
    struct _mstdnt_generic_args history_args = {
        .arg = &(tag->history),
        .size = &(tag->history_len)
    };
    
    memset(tag, 0, sizeof(struct mstdnt_tag));

    struct _mstdnt_val_ref refs[] = {
        { "name", &(tag->name), _mstdnt_val_string_call },
        { "url", &(tag->url), _mstdnt_val_string_call },
        { "history", &history_args, _mstdnt_val_histories_call },
    };

    for (cJSON* v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    
    return 0;
}

// Generated function
GENERATE_JSON_ARRAY_FUNC(mstdnt_tags_json, struct mstdnt_tag, mstdnt_tag_json)

void mstdnt_cleanup_tag(struct mstdnt_tag* tag)
{
    if (tag->history)
        free(tag->history);
}

void mstdnt_cleanup_tags(struct mstdnt_tag* tags, size_t s)
{
    if (!tags) return;
    for (size_t i = 0; i < s; ++i)
    {
        mstdnt_cleanup_tag(tags + i);
    }
    free(tags);
}
