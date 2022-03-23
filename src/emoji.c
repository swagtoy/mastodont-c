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

#include <stdlib.h>
#include <mastodont_json_helper.h>
#include <mastodont_emoji.h>

static void load_emoji_reacts_from_json(struct mstdnt_emoji_reaction* emo, cJSON* emo_json)
{
    cJSON* it;
    struct _mstdnt_val_ref refs[] = {
        { "name", &(emo->name), _mstdnt_val_string_call },
        { "count", &(emo->count), _mstdnt_val_uint_call },
        { "me", &(emo->me), _mstdnt_val_bool_call },
    };

    for (it = emo_json; it; it = it->next)
    {
        _mstdnt_key_val_ref(it, refs, _mstdnt_arr_len(refs));
    }
}

void _mstdnt_val_emoji_reactions_call(cJSON* v, void* _type)
{
    struct _mstdnt_generic_args* args = _type;
    struct mstdnt_emoji_reaction** emos = args->arg;
    cJSON* v_array = v->child;
    cJSON* att = NULL;

    size_t size = cJSON_GetArraySize(v);
    *(args->size) = size;
    /* No attachments, ignore */
    if (size == 0)
    {
        *emos = NULL;
        return;
    }

    *emos = calloc(1, sizeof(struct mstdnt_emoji_reaction) * size);
    if (*emos == NULL)
        return;

    cJSON* it;
    int i;
    for (it = v_array, i = 0; it; (++i, it = it->next))
    {
        load_emoji_reacts_from_json((*emos) + i, it->child);
    }
}

void cleanup_emoji_reaction(struct mstdnt_emoji_reaction* reaction)
{
    /* NOP, this will be implemented soon*/
    return;
}

void cleanup_emoji_reactions(struct mstdnt_emoji_reaction* reactions, size_t s)
{
    size_t i;
    if (!reactions) return;
    for (i = 0; i < s; ++i)
        cleanup_emoji_reaction(reactions + s);
    free(reactions);
}
