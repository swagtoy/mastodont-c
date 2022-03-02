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

#ifndef MASTODONT_PLEROMA
#define MASTODONT_PLEROMA
#include <mastodont_types.h>
#include <mastodont_emoji.h>

struct mstdnt_status_pleroma
{
    /* content */
    int conversation_id;
    int direct_conversation_id;
    char* expires_at;
    char* in_reply_to_account_acct;
    struct mstdnt_emoji_reaction* emoji_reactions;
    size_t emoji_reactions_len;
    mstdnt_bool local;
    mstdnt_bool parent_visible;
    char* pinned_at;
    /* spoiler text */
    int thread_muted;
};

void cleanup_status_pleroma(struct mstdnt_status_pleroma* pleroma);
int mstdnt_load_status_pleroma_from_json(struct mstdnt_status_pleroma* pleroma, cJSON* js);
void _mstdnt_val_status_pleroma_call(cJSON* v, void* _type);

#endif /* MASTODONT_PLEROMA */
