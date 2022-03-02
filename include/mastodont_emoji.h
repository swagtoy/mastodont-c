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

#ifndef MASTODONT_EMOJI
#define MASTODONT_EMOJI
#include "mastodont_types.h"

struct mstdnt_emoji
{
    /* Required */
    char* shortcode;
    char* url;
    char* static_url;
    mstdnt_bool visible_in_picker;

    /* Optional */
    char* category;
};

struct mstdnt_emoji_reaction
{
    char* name;
    size_t count;
    mstdnt_bool me;
    /* TODO Accounts */
};

void cleanup_emoji_reaction(struct mstdnt_emoji_reaction* reactions);
void cleanup_emoji_reactions(struct mstdnt_emoji_reaction* reactions, size_t s);
void _mstdnt_val_emoji_reactions_call(cJSON* v, void* _type);

#endif /* MASTODONT_EMOJI */
