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

/*
 * Originally, when the arguments were being designed for each function,
 * I found that many REST operations tended to result similar variable names
 * under the same types. To reduce the amount of duplicate code, and to even
 * allow argument reusing between multiple functions, all the args are put
 * into one struct, this makes it quite a large struct, but any machine will
 * handle it fine.
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
    int remote;
    int local;
};

#endif /* MASTODONT_ARGUMENTS_H */
