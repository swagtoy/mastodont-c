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

#ifndef MASTODONT_VISIBILITY_TYPES_H
#define MASTODONT_VISIBILITY_TYPES_H
#include <stdint.h>

/* These used to uint8_t's, but because lists are custom strings,
   it was better to make these regular strings */
#define MSTDNT_STR_VISIBILITY_PUBLIC "public"
#define MSTDNT_STR_VISIBILITY_UNLISTED "unlisted"
#define MSTDNT_STR_VISIBILITY_PRIVATE "private"
#define MSTDNT_STR_VISIBILITY_DIRECT "direct"
#define MSTDNT_STR_VISIBILITY_LOCAL "local"

typedef char* mstdnt_visibility_t;

enum mstdnt_visibility_type
{
    MSTDNT_VISIBILITY_PUBLIC,
    MSTDNT_VISIBILITY_UNLISTED,
    MSTDNT_VISIBILITY_PRIVATE,
    MSTDNT_VISIBILITY_DIRECT,
    MSTDNT_VISIBILITY_LOCAL,
    MSTDNT_VISIBILITY_LIST,
    MSTDNT_VISIBILITY_UNKNOWN,
};

#endif /* MASTODONT_VISIBILITY_TYPES_H */
