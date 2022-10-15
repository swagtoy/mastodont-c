/*
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_attachment.h>
#include <mastodont_json_helper.h>
#include <mastodont_request.h>
#include <mastodont_query.h>

void _mstdnt_val_attachment_type_call(cJSON* v, void* _type)
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

int mstdnt_attachment_json(cJSON* att_json, struct mstdnt_attachment* att)
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

void _mstdnt_val_attachments_call(cJSON* v, void* _type)
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

static int mstdnt_attachment_json_callback(cJSON* json, void* _args)
{
    return mstdnt_attachment_json(json, _args);
}

int mstdnt_upload_media(mastodont_t* api,
                           struct mstdnt_args* m_args,
                           struct mstdnt_upload_media_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_attachment* attachment)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_FILE, "file", { .f = &(args->file) } },
        { _MSTDNT_QUERY_FILE, "thumbnail", { .f = args->thumbnail } }
    };
    
    struct mstdnt_request_args req_args = {
        storage,
        "api/v1/media",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_MIMEPOST,
        NULL,
        attachment,
        mstdnt_attachment_json_callback,
    };

    return mstdnt_request(api, m_args, &req_args);
}

void mstdnt_cleanup_attachments(struct mstdnt_attachment* attachment)
{
    if (attachment) mstdnt_free(attachment);
}
