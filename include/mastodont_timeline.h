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

#ifndef MASTODONT_TIMELINE_H
#define MASTODONT_TIMELINE_H
#include <mastodont_types.h>
#include <mastodont_status.h>

enum mstdnt_reply_visibility
{
    MSTDNT_REPLY_VIS_NONE = 0,
    MSTDNT_REPLY_VIS_SELF,
    MSTDNT_REPLY_VIS_FOLLOWING,
};

struct mstdnt_timeline_args
{
    int only_media;
    char* instance;
    int remote;
    enum mstdnt_reply_visibility reply_visibility;
    char* max_id;
    char* since_id;
    char* min_id;
    int limit;
    int offset;
    int local;
    int with_muted;
};

int mstdnt_timeline_home(mstdnt_t* data,
                            struct mstdnt_args* m_args,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size);

int mstdnt_timeline_list(mstdnt_t* data,
                            struct mstdnt_args* m_args,
                            char* list_id,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size);

int mstdnt_timeline_public(mstdnt_t* data,
                              struct mstdnt_args* m_args,
                              struct mstdnt_timeline_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* statuses_size);

int mstdnt_timeline_direct(mstdnt_t* data,
                              struct mstdnt_args* m_args,
                              struct mstdnt_timeline_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* statuses_size);

int mstdnt_timeline_tag(mstdnt_t* data,
                           struct mstdnt_args* m_args,
                           char* hashtag,
                           struct mstdnt_timeline_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_status* statuses[],
                           size_t* size);

#endif /* MASTODONT_TIMELINE_H */
