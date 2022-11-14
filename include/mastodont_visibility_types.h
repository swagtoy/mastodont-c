/*
 * Licensed under BSD 3-Clause License
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
    MSTDNT_VISIBILITY_UNKNOWN,
    MSTDNT_VISIBILITY_PUBLIC,
    MSTDNT_VISIBILITY_UNLISTED,
    MSTDNT_VISIBILITY_PRIVATE,
    MSTDNT_VISIBILITY_LIST,
    MSTDNT_VISIBILITY_DIRECT,
    MSTDNT_VISIBILITY_LOCAL,
};

#endif /* MASTODONT_VISIBILITY_TYPES_H */
