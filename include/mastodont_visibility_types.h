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

#define MSTDNT_VISIBILITY_PUBLIC (1<<0)
#define MSTDNT_VISIBILITY_UNLISTED (1<<1)
#define MSTDNT_VISIBILITY_PRIVATE (1<<2)
#define MSTDNT_VISIBILITY_DIRECT (1<<3)
#define MSTDNT_VISIBILITY_LIST (1<<4)
#define MSTDNT_VISIBILITY_LOCAL (1<<5)

typedef mstdnt_visibility_t uint8_t;

#endif /* MASTODONT_VISIBILITY_TYPES_H */
