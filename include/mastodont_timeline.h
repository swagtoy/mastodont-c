/*
 * Licensed under BSD 2-Clause License
 */

#ifndef MASTODONT_TIMELINE_H
#define MASTODONT_TIMELINE_H
#include <mastodont_types.h>
#include <mastodont_status.h>

enum mstdnt_reply_visibility
{
    MSTDNT_REPLY_VIS_NONE = 0,
    MSTDNT_REPLY_VIS_SELF,
    MSTDNT_REPLY_VIS_FOLLOWING,
};

struct mstdnt_timeline_args
{
    int only_media;
    char* instance;
    int remote;
    enum mstdnt_reply_visibility reply_visibility;
    char* max_id;
    char* since_id;
    char* min_id;
    int limit;
    int offset;
    int local;
    int with_muted;
};

int
mstdnt_timeline_home(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     struct mstdnt_timeline_args args);

int
mstdnt_timeline_list(mastodont_t* data,
                     struct mstdnt_args* m_args,
                     mstdnt_request_cb_t cb_request,
                     void* cb_args,
                     char* list_id,
                     struct mstdnt_timeline_args args);

int
mstdnt_timeline_public(mastodont_t* data,
                       struct mstdnt_args* m_args,
                       mstdnt_request_cb_t cb_request,
                       void* cb_args,
                       struct mstdnt_timeline_args args);

int
mstdnt_timeline_direct(mastodont_t* data,
                       struct mstdnt_args* m_args,
                       mstdnt_request_cb_t cb_request,
                       void* cb_args,
                       struct mstdnt_timeline_args args);

int
mstdnt_timeline_tag(mastodont_t* data,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    char* hashtag,
                    struct mstdnt_timeline_args args);

#endif /* MASTODONT_TIMELINE_H */
