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

#ifndef MASTODONT_INSTANCE_H
#define MASTODONT_INSTANCE_H
#include "mastodont_types.h"

struct mstdnt_instance
{
    int a;
};

// TODO
int mstdnt_instance(mstdnt_t* api, struct mstdnt_args* args,
                       struct mstdnt_instance* inst);

#endif /* MASTODONT_INSTANCE_H */
