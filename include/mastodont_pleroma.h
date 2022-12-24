/*
 * Licensed under BSD 3-Clause License
 */


// TODO Move me to statuses
#ifndef MASTODONT_PLEROMA
#define MASTODONT_PLEROMA
#include "mastodont_types.h"
#include "mastodont_emoji.h"

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

void mstdnt_cleanup_status_pleroma(struct mstdnt_status_pleroma* pleroma);
int mstdnt_status_pleroma_from_json(struct mstdnt_status_pleroma* pleroma, cJSON* js);
void _mstdnt_val_status_pleroma_call(cJSON* v, void* _type);

#endif /* MASTODONT_PLEROMA_H */
