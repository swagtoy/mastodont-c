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

#include <mastodont_json_helper.h>
#include <mastodont_pleroma.h>

int mstdnt_status_pleroma_from_json(struct mstdnt_status_pleroma* pleroma, cJSON* js)
{
    cJSON* v;

    struct _mstdnt_generic_args emo_args = {
        &(pleroma->emoji_reactions),
        &(pleroma->emoji_reactions_len),
    };

    struct _mstdnt_val_ref refs[] = {
        { "conversation_id", &(pleroma->conversation_id), _mstdnt_val_sint_call },
        { "direct_conversation_id", &(pleroma->direct_conversation_id), _mstdnt_val_sint_call },
        { "expires_at", &(pleroma->expires_at), _mstdnt_val_string_call },
        { "in_reply_to_account_acct", &(pleroma->in_reply_to_account_acct), _mstdnt_val_string_call },
        { "emoji_reactions", &emo_args, _mstdnt_val_emoji_reactions_call },
        { "pinned_at", &(pleroma->pinned_at), _mstdnt_val_string_call },
        { "thread_muted", &(pleroma->thread_muted), _mstdnt_val_sint_call },
    };

    for (v = js; v; v = v->next)
    {
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    }
}

void _mstdnt_val_status_pleroma_call(cJSON* v, void* _type)
{
    struct mstdnt_status_pleroma* type = _type;

    mstdnt_status_pleroma_from_json(type, v->child);
}

void cleanup_status_pleroma(struct mstdnt_status_pleroma* pleroma)
{
    if (!pleroma) return;
    cleanup_emoji_reactions(pleroma->emoji_reactions, pleroma->emoji_reactions_len);
}

