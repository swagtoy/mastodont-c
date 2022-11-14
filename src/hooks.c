/*
 * Licensed under BSD 2-Clause License
 */

#include <stdlib.h>
#include <assert.h>
#include <cjson/cJSON.h>
#include <mastodont_hooks.h>

struct mstdnt_hooks _mstdnt_hooks_def = {
    .malloc = malloc,
    .free = free,
    .calloc = calloc,
    .realloc = realloc,
};

void mstdnt_set_hooks(struct mstdnt_hooks* hooks)
{
    assert(hooks != NULL);
    cJSON_Hooks cjson_hooks = {
        .malloc_fn = hooks->malloc,
        .free_fn = hooks->free,
    };
    cJSON_InitHooks(&cjson_hooks);

    if (hooks->malloc)
        _mstdnt_hooks_def.malloc = hooks->malloc;
    if (hooks->free)
        _mstdnt_hooks_def.free = hooks->free;
    if (hooks->calloc)
        _mstdnt_hooks_def.calloc = hooks->calloc;
    if (hooks->realloc)
        _mstdnt_hooks_def.realloc = hooks->realloc;
}

