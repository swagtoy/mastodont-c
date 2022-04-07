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
#include <mastodont_attachment.h>
#include <mastodont_json_helper.h>
#include <mastodont_request.h>
#include <mastodont_query.h>

void load_attachment_from_json(struct mstdnt_attachment* att, cJSON* att_json)
{
    cJSON* it;

    /* Zero out */
    memset(att, 0, sizeof(struct mstdnt_attachment));
    
    struct _mstdnt_val_ref refs[] = {
        { "id", &(att->id), _mstdnt_val_string_call },
        /* TODO type */
        { "url", &(att->url), _mstdnt_val_string_call },
        { "preview_url", &(att->preview_url), _mstdnt_val_string_call },
        { "remote_url", &(att->remote_url), _mstdnt_val_string_call },
        { "hash", &(att->hash), _mstdnt_val_string_call },
        { "description", &(att->description), _mstdnt_val_string_call },
        { "blurhash", &(att->blurhash), _mstdnt_val_string_call },        
    };

    for (it = att_json; it; it = it->next)
    {
        _mstdnt_key_val_ref(it, refs, _mstdnt_arr_len(refs));
    }
}

int mstdnt_attachment_result(struct mstdnt_fetch_results* results,
                             struct mstdnt_storage* storage,
                             struct mstdnt_attachment* att)
{
    /* Can be null sometimes */
    if (!att) return 0;
    
    cJSON* root;
    if (_mstdnt_json_init(&root, results, storage) ||
        !cJSON_IsObject(root))
        return 1;

    load_attachment_from_json(att, root->child);
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

    *attachments = calloc(1, sizeof(struct mstdnt_attachment) * size);
    if (*attachments == NULL)
        return;

    cJSON* it;
    int i;
    for (it = v_array, i = 0; it; (++i, it = it->next))
    {
        load_attachment_from_json((*attachments) + i, it->child);
    }
}

static int mstdnt_attachment_callback(struct mstdnt_fetch_results* results,
                                      struct mstdnt_storage* storage,
                                      void* _args)
{
    return mstdnt_attachment_result(results, storage, _args);
}

int mastodont_upload_media(mastodont_t* api,
                           struct mstdnt_upload_media_args* args,
                           struct mstdnt_storage* storage,
                           struct mstdnt_attachment* attachment)
{
    union param_value u_file, u_thumbnail, u_description;
    u_file.s = args->file;
    u_thumbnail.s = args->thumbnail;
    u_description.s = args->description;

    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_STRING, "file", u_file },
        { _MSTDNT_QUERY_STRING, "thumbnail", u_thumbnail },
        { _MSTDNT_QUERY_STRING, "description", u_description },
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/media",
        NULL, 0,
        params, _mstdnt_arr_len(params),
        CURLOPT_MIMEPOST,
        attachment,
        mstdnt_attachment_callback,
    };

    return mastodont_request(api, &req_args);
}

void cleanup_attachments(struct mstdnt_attachment* attachment)
{
    if (attachment) free(attachment);
}
