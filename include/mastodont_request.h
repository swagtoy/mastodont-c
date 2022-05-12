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

#ifndef MASTODONT_REQUEST_H
#define MASTODONT_REQUEST_H
#include <cjson/cJSON.h>
#include "mastodont_types.h"
#include "mastodont_fetch.h"
#include "mastodont_query.h"

struct mastodont_request_args
{
    struct mstdnt_storage* storage;
    char* url;
    struct _mstdnt_query_param* params_query;
    size_t params_query_len;
    struct _mstdnt_query_param* params_post;
    size_t params_post_len;
    CURLoption request_type;
    void* args;
    int (*callback)(cJSON*, void*);
};

int mastodont_request(mastodont_t* data, struct mastodont_request_args* args);

#endif /* MASTODONT_REQUEST_H */
