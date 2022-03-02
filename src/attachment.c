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

#include <stdlib.h>
#include <mastodont_attachment.h>
#include <mastodont_json_helper.h>

static void load_attachment_from_json(struct mstdnt_attachment* att, cJSON* att_json)
{
    cJSON* it;
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

    *attachments = malloc(sizeof(struct mstdnt_attachment) * size);
    if (*attachments == NULL)
        return;

    cJSON* it;
    int i;
    for (it = v_array, i = 0; it; (++i, it = it->next))
    {
        load_attachment_from_json((*attachments) + i, it->child);
    }
}

void cleanup_attachments(struct mstdnt_attachment* attachment)
{
    if (attachment) free(attachment);
}
