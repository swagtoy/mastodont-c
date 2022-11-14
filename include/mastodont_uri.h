/*
 * Licensed under BSD 2-Clause License
 */

#ifndef MASTODONT_URI_H
#define MASTODONT_URI_H
#include <mastodont_types.h>
#include <stddef.h>

struct mstdnt_uri
{
    char* user;
    char* domain;
    char storage[MSTDNT_URISIZE];
};

struct mstdnt_uri mstdnt_uristr_to_uri(char* uri, size_t len);

#endif /* MASTODONT_URI_H */
