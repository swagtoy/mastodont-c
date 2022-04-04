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

#ifndef MASTODONT_RELATIONSHIP_H
#define MASTODONT_RELATIONSHIP_H
#include <mastodont_types.h>
#include <mastodont_request.h>

struct mstdnt_relationship
{
    char* id;
    mstdnt_bool following;
    mstdnt_bool requested;
    mstdnt_bool endorsed;
    mstdnt_bool followed_by;
    mstdnt_bool muting;
    mstdnt_bool muting_notifications;
    mstdnt_bool showing_reblogs;
    mstdnt_bool notifying;
    mstdnt_bool blocking;
    mstdnt_bool domain_blocking;
    mstdnt_bool blocked_by;
    char* note;
};

int mstdnt_relationships_result(struct mstdnt_fetch_results* results,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships,
                                size_t* size);

int _mstdnt_relationships_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args);

int mastodont_get_relationships(mastodont_t* data,
                                char** ids,
                                size_t ids_len,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships[],
                                size_t* size);

#endif /* MASTODONT_RELATIONSHIP_H */
