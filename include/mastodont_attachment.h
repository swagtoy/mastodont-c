/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_ATTACHMENT
#define MASTODONT_ATTACHMENT
#include "mastodont_types.h"
#include "mastodont_fetch.h"

#ifdef __cplusplus
extern "C" {
#endif

enum mstdnt_attachment_type
{
    MSTDNT_ATTACHMENT_UNKNOWN,
    MSTDNT_ATTACHMENT_IMAGE,
    MSTDNT_ATTACHMENT_GIFV,
    MSTDNT_ATTACHMENT_VIDEO,
    MSTDNT_ATTACHMENT_AUDIO
};

struct mstdnt_attachment
{
    char* id;
    enum mstdnt_attachment_type type;
    char* url;
    char* preview_url;
    char* remote_url;
    char* hash; /* <- Likely wrong */
    char* description;
    char* blurhash;
};

struct mstdnt_upload_media_args
{
    struct mstdnt_file file;
    struct mstdnt_file* thumbnail;
    char* description;
    /* TODO focus */
};

int
mstdnt_attachment_json(cJSON* att_json, struct mstdnt_attachment* att);

void
_mstdnt_val_attachments_call(cJSON* v, void* _type);

int
mstdnt_upload_media(mastodont_t* api,
                        struct mstdnt_args* m_args,
                        mstdnt_request_cb_t cb_request,
                        void* cb_args,
                        struct mstdnt_upload_media_args args);

void
mstdnt_cleanup_attachments(struct mstdnt_attachment* attachment);

void
_mstdnt_val_attachments_call(cJSON* v, void* _type);

#ifdef __cplusplus
}
#endif

#endif /* MASTODONT_ATTACHMENT */
