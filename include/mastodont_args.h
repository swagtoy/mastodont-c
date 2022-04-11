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

#ifndef MASTODONT_ARGUMENTS_H
#define MASTODONT_ARGUMENTS_H
#include "mastodont_notif_types.h"

/*
 * TODO undo this shit, it's actually just gross i'll duplicate the code i do not care
 */

struct mstdnt_args
{
    char* client_name;
    char* redirect_uris;
    char* scopes;
    char* website;
    char* grant_type;
    char* client_id;
    char* client_secret;
    char* redirect_uri;
    char* scope;
    char* code;
    char* username;
    char* password;
    int pinned;
    char* tagged;
    int with_muted;
    int exclude_reblogs;
    int exclude_replies;
    int offset;
    int only_media;
    char* max_id;
    char* since_id;
    char* min_id;
    int limit;
    char* content_type;
    int expires_in;
    char* in_reply_to_conversation_id;
    char* in_reply_to_id;
    char* language;
    char** media_ids;
    void* poll; /* TODO */
    int preview;
    char* scheduled_at;
    int sensitive;
    char* spoiler_text;
    char* status;
    char* visibility;
    int media_ids_len;
    int remote;
    int local;
    char** exclude_types;
    size_t exclude_types_len;
    char* account_id;
    char** exclude_visibilities;
    size_t exclude_visibilities_len;
    enum mstdnt_notification_type* include_types;
};

#endif /* MASTODONT_ARGUMENTS_H */
