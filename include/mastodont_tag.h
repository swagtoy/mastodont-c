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
