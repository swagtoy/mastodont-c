/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_URI_H
#define MASTODONT_URI_H
#include "mastodont_types.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mstdnt_uri
{
    char* user;
    char* domain;
    char storage[MSTDNT_URISIZE];
};

struct mstdnt_uri mstdnt_uristr_to_uri(char* uri, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* MASTODONT_URI_H */
