/*
 * Licensed under BSD 2-Clause License
 */

#include <mastodont_uri.h>
#include <string.h>

struct mstdnt_uri mstdnt_uristr_to_uri(char* uri, size_t len)
{
    int i;
    struct mstdnt_uri ret;
    if (len <= 0)
        len = strlen(uri);

    ret.user = ret.storage;
    /* Copy information over */
    for (i = 0; i < len; ++i)
    {
        switch (uri[i])
        {
        case '@':
            ret.storage[i] = '\0';
            ret.domain = ret.storage + i + 1;
            break;
        default:
            ret.storage[i] = uri[i];
            break;
        }
    }
    
    ret.storage[i] = '\0';
    return ret;
}
