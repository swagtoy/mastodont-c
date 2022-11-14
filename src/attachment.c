/*
 * Licensed under BSD 2-Clause License
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_attachment.h>
#include <mastodont_json_helper.h>
#include <mastodont_request.h>
#include <mastodont_query.h>

void
_mstdnt_val_attachment_type_call(cJSON* v, void* _type)
{
    enum mstdnt_attachment_type* type = _type;
    if (!cJSON_IsString(v))
    {
        *type = MSTDNT_ATTACHMENT_UNKNOWN;
        return;
    }

    if (strcmp(v->valuestring, "unknown") == 0)
        *type = MSTDNT_ATTACHMENT_UNKNOWN;
    else if (strcmp(v->valuestring, "image") == 0)
        *type = MSTDNT_ATTACHMENT_IMAGE;
    else if (strcmp(v->valuestring, "gifv") == 0)
        *type = MSTDNT_ATTACHMENT_GIFV;
    else if (strcmp(v->valuestring, "video") == 0)
        *type = MSTDNT_ATTACHMENT_VIDEO;
    else if (strcmp(v->valuestring, "audio") == 0)
        *type = MSTDNT_ATTACHMENT_AUDIO;
}

int
mstdnt_attachment_json(cJSON* att_json,
                           struct mstdnt_attachment* att)
{
    if (!att) return 1;
    
    /* Zero out */
    memset(att, 0, sizeof(struct mstdnt_attachment));
    
    struct _mstdnt_val_ref refs[] = {
        { "id", &(att->id), _mstdnt_val_string_call },
        { "type", &(att->type), _mstdnt_val_attachment_type_call },
        { "url", &(att->url), _mstdnt_val_string_call },
        { "preview_url", &(att->preview_url), _mstdnt_val_string_call },
        { "remote_url", &(att->remote_url), _mstdnt_val_string_call },
        { "hash", &(att->hash), _mstdnt_val_string_call },
        { "description", &(att->description), _mstdnt_val_string_call },
        { "blurhash", &(att->blurhash), _mstdnt_val_string_call },        
    };

    for (cJSON* it = att_json->child; it; it = it->next)
        _mstdnt_key_val_ref(it, refs, _mstdnt_arr_len(refs));

    return 0;
}

void
_mstdnt_val_attachments_call(cJSON* v, void* _type)
{
    struct _mstdnt_generic_args* args = _type;
    struct mstdnt_attachment** attachments = args->arg;
    cJSON* v_array = v->child;
    cJSON* att = NULL;

    size_t size = cJSON_GetArraySize(v);
    *(args->size) = size;
    /* No attachments, ignore */
    if (size == 0)
    {
        *attachments = NULL;
        return;
    }

    *attachments = mstdnt_calloc(1, sizeof(struct mstdnt_attachment) * size);
    if (*attachments == NULL)
        return;

    cJSON* it;
    int i;
    for (it = v_array, i = 0; it; (++i, it = it->next))
    {
        mstdnt_attachment_json(it, (*attachments) + i);
    }
}

static int
mstdnt_attachment_json_callback(cJSON* json,
                                void* args,
                                mstdnt_request_cb_data* data)
{
    (void)args;

    struct mstdnt_attachment* attachment = malloc(sizeof(struct mstdnt_attachment));
    data->data = attachment;
    // Intentionally not set
    //data->data_free_cb
    return mstdnt_attachment_json(json, attachment);
}

int
mstdnt_upload_media(mastodont_t* api,
                    struct mstdnt_args* m_args,
                    mstdnt_request_cb_t cb_request,
                    void* cb_args,
                    struct mstdnt_upload_media_args args)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_FILE, "file", { .f = &(args.file) } },
        { _MSTDNT_QUERY_FILE, "thumbnail", { .f = args.thumbnail } }
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/media",
        .params_post = params,
        .params_post_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_MIMEPOST,
        .callback = mstdnt_attachment_json_callback,
    };

    return mstdnt_request(api, m_args, cb_request, cb_args, &req_args);
}

void
mstdnt_cleanup_attachments(struct mstdnt_attachment* attachment)
{
    if (attachment) mstdnt_free(attachment);
}
