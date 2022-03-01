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

#ifndef MASTODONT_ATTACHMENT
#define MASTODONT_ATTACHMENT
#include "mastodont_types.h"
/* Status: Complete, not implemented */

enum mstdnt_attachment_type
{
    MSTDNT_ATTACHMENT_UNKNOWN,
    MSTDNT_ATTACHMENT_IMAGE,
    MSTDNT_ATTACHMENT_GIFV,
    MSTDNT_ATTACHMENT_VIDEO,
    MSTDNT_ATTACHMENT_AUDIO
};

struct mstdnt_attachment
{
    char* id;
    enum mstdnt_attachment_type type;
    char* url;
    char* preview_url;
    char* remote_url;
    char* hash; /* <- Likely wrong */
    char* description;
    char* blurhash;
};

void _mstdnt_val_attachments_call(cJSON* v, void* _type);

#endif /* MASTODONT_ATTACHMENT */
