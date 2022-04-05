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
#include <stdint.h>
#include <mastodont_types.h>
#include <mastodont_request.h>

typedef uint16_t mstdnt_relationship_flag_t;

#define MSTDNT_RELATIONSHIP_NOOP 0
#define MSTDNT_RELATIONSHIP_FOLLOWING (1<<0)
#define MSTDNT_RELATIONSHIP_REQUESTED (1<<1)
#define MSTDNT_RELATIONSHIP_ENDORSED (1<<2)
#define MSTDNT_RELATIONSHIP_FOLLOWED_BY (1<<3)
#define MSTDNT_RELATIONSHIP_MUTING (1<<4)
#define MSTDNT_RELATIONSHIP_MUTING_NOTIFS (1<<5)
#define MSTDNT_RELATIONSHIP_SHOWING_REBLOGS (1<<6)
#define MSTDNT_RELATIONSHIP_NOTIFYING (1<<7)
#define MSTDNT_RELATIONSHIP_BLOCKING (1<<8)
#define MSTDNT_RELATIONSHIP_DOMAIN_BLOCKING (1<<9)
#define MSTDNT_RELATIONSHIP_BLOCKED_BY (1<<10)

struct mstdnt_relationship
{
    char* id;
    mstdnt_relationship_flag_t flags;
    char* note;
};

int mstdnt_relationship_json(struct mstdnt_relationship* relationship, cJSON* js);

int mstdnt_relationships_result(struct mstdnt_fetch_results* results,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships[],
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
