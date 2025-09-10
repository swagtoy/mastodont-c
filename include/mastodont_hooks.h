/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_HOOKS_H
#define MASTODONT_HOOKS_H
#include <stddef.h>

#define mstdnt_malloc (*_mstdnt_hooks_def.malloc)
#define mstdnt_free (*_mstdnt_hooks_def.free)
#define mstdnt_calloc (*_mstdnt_hooks_def.calloc)
#define mstdnt_realloc (*_mstdnt_hooks_def.realloc)

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // MASTODONT_HOOKS_H
