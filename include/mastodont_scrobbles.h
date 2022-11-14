/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_SCROBBLES_H
#define MASTODONT_SCROBBLES_H
#include <stddef.h>
#include <mastodont.h>
#include <mastodont_types.h>
#include <mastodont_account.h>

typedef struct mstdnt_scrobble
{
    struct mstdnt_account account;
    char* album;
    char* artist;
    time_t created_at;
    char* id;
    int length;
    char* title;
} mstdnt_scrobble;

typedef struct mstdnt_scrobbles
{
    mstdnt_scrobble* scrobbles;
    size_t len;
} mstdnt_scrobbles;

struct mstdnt_get_scrobbles_args
{
    char* max_id;
    char* min_id;
    char* since_id;
    int offset;
    int limit;
};

// DEPRECATED 
struct _mstdnt_scrobbles_cb_args
{
    struct mstdnt_scrobble** scrobbles;
    size_t* size;
};

int
mstdnt_get_scrobbles(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* id,
                     struct mstdnt_get_scrobbles_args args);

int
mstdnt_scrobbles_json(struct mstdnt_scrobble* scrobbles[],
                      size_t* size,
                      cJSON* json);

int
mstdnt_scrobbles_json_callback(cJSON* json,
                               void* arg,
                               mstdnt_request_cb_data* data);

int
mstdnt_scrobble_json(struct mstdnt_scrobble* scrobble, cJSON* js);

void
mstdnt_cleanup_scrobble(mstdnt_scrobble* scrobble);

void
mstdnt_cleanup_scrobbles(mstdnt_scrobbles* scrobbles);

#endif /* MASTODONT_SCROBBLES_H */
