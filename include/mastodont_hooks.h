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

#ifndef MASTODONT_HOOKS_H
#define MASTODONT_HOOKS_H
#include <stddef.h>

#define mstdnt_malloc (*_mstdnt_hooks_def.malloc)
#define mstdnt_free (*_mstdnt_hooks_def.free)
#define mstdnt_calloc (*_mstdnt_hooks_def.calloc)
#define mstdnt_realloc (*_mstdnt_hooks_def.realloc)

struct mstdnt_hooks
{
    void* (*malloc)(size_t size);
    void  (*free)(void* ptr);
    void* (*calloc)(size_t nmemb, size_t size);
    void* (*realloc)(void* ptr, size_t size);
};

/**
 * Sets up mstdnt_hooks (and cJSON hooks)
 *
 * @param data Pointer to the mstdnt data
 */
void mstdnt_set_hooks(struct mstdnt_hooks* data);

/** Hooks for Mastodont functions */
extern struct mstdnt_hooks _mstdnt_hooks_def;

#endif // MASTODONT_HOOKS_H
