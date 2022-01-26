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
#include <mastodont_status.h>

int mstdnt_load_status_from_json(struct mstdnt_status* status, cJSON* js)
{
    cJSON* v;
    for (v = js; v; v = v->next)
    {
        if (cJSON_IsString(v))
        {
            if(strcmp("id", v->string)==0) status->id = v->valuestring;
            if(strcmp("uri", v->string)==0) status->uri = v->valuestring;
            if(strcmp("created_at", v->string)==0) status->created_at = v->valuestring;
            if(strcmp("content", v->string)==0) status->content = v->valuestring;
            if(strcmp("spoiler_text", v->string)==0) status->spoiler_text = v->valuestring;
            if(strcmp("in_reply_to_id", v->string)==0) status->in_reply_to_id = v->valuestring;
            if(strcmp("in_reply_to_account_id", v->string)==0) status->in_reply_to_account_id = v->valuestring;
            if(strcmp("in_reply_to_id", v->string)==0) status->in_reply_to_id = v->valuestring;
            if(strcmp("language", v->string)==0) status->language = v->valuestring;
            if(strcmp("text", v->string)==0) status->text = v->valuestring;
            if(strcmp("in_reply_to_id", v->string)==0) status->in_reply_to_id = v->valuestring;
        }
        else if (cJSON_IsBool(v))
        {
            /* It's probably an int 1/0, but for portability reasons I'll use the typedefs */
            const int val = cJSON_IsTrue(v) ? cJSON_True : cJSON_False;
            if (strcmp("sensitive", v->string) == 0) status->sensitive = val;
            if (strcmp("favourited", v->string) == 0) status->favourited = val;
            if (strcmp("reblogged", v->string) == 0) status->reblogged = val;
            if (strcmp("muted", v->string) == 0) status->sensitive = val;
            if (strcmp("bookmarked", v->string) == 0) status->bookmarked = val;
            if (strcmp("pinned", v->string) == 0) status->pinned = val;
        }
    }
}
