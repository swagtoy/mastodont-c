/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_TAG
#define MASTODONT_TAG
#include <stdint.h>
#include "mastodont_history.h"

struct mstdnt_tag
{
    char* name;
    char* url;
    struct mstdnt_history* history;
    size_t history_len;
};

int mstdnt_tag_json(struct mstdnt_tag* tag, cJSON* emo_json);
int mstdnt_tags_json(struct mstdnt_tag* array[], size_t* array_size, cJSON* js);
void mstdnt_cleanup_tag(struct mstdnt_tag* tag);
void mstdnt_cleanup_tags(struct mstdnt_tag* tags, size_t s);

#endif /* MASTODONT_TAG */
