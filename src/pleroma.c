/*
 * Licensed under BSD 3-Clause License
 */

#include <string.h>
#include <mastodont_json_helper.h>
#include <mastodont_pleroma.h>

int mstdnt_status_pleroma_from_json(struct mstdnt_status_pleroma* pleroma, cJSON* js)
{
    cJSON* v;

    /* Zero out */
    memset(pleroma, 0, sizeof(struct mstdnt_status_pleroma));

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

    return 0;
}

void _mstdnt_val_status_pleroma_call(cJSON* v, void* _type)
{
    struct mstdnt_status_pleroma* type = _type;

    mstdnt_status_pleroma_from_json(type, v->child);
}

void mstdnt_cleanup_status_pleroma(struct mstdnt_status_pleroma* pleroma)
{
    if (!pleroma) return;
    mstdnt_cleanup_emoji_reactions(pleroma->emoji_reactions, pleroma->emoji_reactions_len);
}

