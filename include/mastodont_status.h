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

#ifndef MASTODONT_STATUS
#define MASTODONT_STATUS
#include <cjson/cJSON.h>
#include "mastodont_pleroma.h"
#include "mastodont_types.h"
#include "mastodont_args.h"
#include "mastodont_fetch.h"
#include "mastodont_attachment.h"
#include "mastodont_application.h"
#include "mastodont_mention.h"
#include "mastodont_account.h"
#include "mastodont_emoji.h"
#include "mastodont_tag.h"
#include "mastodont_account.h"
#include "mastodont_pleroma.h"
/* Status: Complete, not implemented */

enum mstdnt_status_visibility
{
    MSTDNT_VISIBILITY_PUBLIC,
    MSTDNT_VISIBILITY_UNLISTED,
    MSTDNT_VISIBILITY_PRIVATE,
    MSTDNT_VISIBILITY_DIRECT,
    MSTDNT_VISIBILITY_LIST,
    MSTDNT_VISIBILITY_LOCAL
};

struct mstdnt_status
{
    char* id;
    char* uri;
    char* created_at;
    struct mstdnt_account account;
    char* content;
    enum mstdnt_status_visibility visibility;
    mstdnt_bool sensitive;
    char* spoiler_text;
    struct mstdnt_attachment* media_attachments;
    size_t media_attachments_len;
    struct mstdnt_application application;
    struct mstdnt_status_pleroma pleroma;

    /* Rendering attributes */
    struct mstdnt_mention* mentions;
    struct mstdnt_tag* tags;
    struct mstdnt_emoji* emojis;

    /* Information attributes */
    unsigned reblogs_count;
    unsigned favourites_count;
    unsigned replies_count;

    /* Nullable attributes */
    char* url;
    char* in_reply_to_id;
    char* in_reply_to_account_id;
    struct mstdnt_status* status;
    struct mstdnt_poll* poll;
    struct mstdnt_card* card;
    char* language;
    char* text;

    /* Authorized user attributes */
    mstdnt_bool favourited;
    mstdnt_bool reblogged;
    mstdnt_bool muted;
    mstdnt_bool bookmarked;
    mstdnt_bool pinned;
};

void cleanup_statuses(struct mstdnt_status* statuses, size_t s);
void cleanup_status(struct mstdnt_status* status);

int mstdnt_statuses_from_result(struct mstdnt_storage* storage,
                                struct mstdnt_fetch_results* results,
                                struct mstdnt_status* status[],
                                size_t* size);

int mstdnt_status_from_result(struct mstdnt_status* status,
                              struct mstdnt_storage* storage,
                              struct mstdnt_fetch_results* results);

int mstdnt_status_from_json(struct mstdnt_status* status, cJSON* js);

int mastodont_get_account_statuses(mastodont_t* data,
                                   char* id,
                                   struct mstdnt_args* args,
                                   struct mstdnt_storage* storage,
                                   struct mstdnt_status* statuses[],
                                   size_t* size);

int mastodont_get_status(mastodont_t* data,
                         char* id,
                         struct mstdnt_storage* storage,
                         struct mstdnt_status* status);

int mastodont_get_status_context(mastodont_t* data,
                                 char* id,
                                 struct mstdnt_storage* storage,
                                 struct mstdnt_status* statuses_before[],
                                 struct mstdnt_status* statuses_after[],
                                 size_t* size_before,
                                 size_t* size_after);
                             
int mastodont_create_status(mastodont_t* data,
                            struct mstdnt_args* args,
                            struct mstdnt_storage* storage);

int mastodont_favourite_status(mastodont_t* data,
                               char* id,
                               struct mstdnt_storage* storage);

int mastodont_reblog_status(mastodont_t* data,
                            char* id,
                            struct mstdnt_storage* storage);

#endif /* MASTODONT_STATUS */
