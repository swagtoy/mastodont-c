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

struct mstdnt_timeline_args
{
    int only_media;
    int remote;    
    char* max_id;
    char* since_id;
    char* min_id;
    int limit;
    int local;
};

int mastodont_timeline_home(mastodont_t* data,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size);

int mastodont_timeline_list(mastodont_t* data,
                            char* list_id,
                            struct mstdnt_timeline_args* args,
                            struct mstdnt_storage* storage,
                            struct mstdnt_status* statuses[],
                            size_t* size);

int mastodont_timeline_public(mastodont_t* data,
                              struct mstdnt_timeline_args* args,
                              struct mstdnt_storage* storage,
                              struct mstdnt_status* statuses[],
                              size_t* statuses_size);

#endif /* MASTODONT_TIMELINE_H */
