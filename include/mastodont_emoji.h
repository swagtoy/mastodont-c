/*
 * Licensed under BSD 2-Clause License
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
    // The following below are for custom emoji reactions
    // They do not exist in Pleroma :^)
    char* url;
    char* static_url;
    /* TODO Accounts */
};

// TODO move static?
void load_emoji_from_json(struct mstdnt_emoji* emo, cJSON* emo_json);
void load_emoji_react_from_json(struct mstdnt_emoji_reaction* emo, cJSON* emo_json);
void _mstdnt_val_emojis_call(cJSON* v, void* _type);
void _mstdnt_val_emoji_reactions_call(cJSON* v, void* _type);
void mstdnt_cleanup_emoji_reaction(struct mstdnt_emoji_reaction* reactions);
void mstdnt_cleanup_emoji_reactions(struct mstdnt_emoji_reaction* reactions, size_t s);
void mstdnt_cleanup_emojis(struct mstdnt_emoji* emo);

#endif /* MASTODONT_EMOJI */
