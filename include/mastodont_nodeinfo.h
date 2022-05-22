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

#ifndef MASTODONT_NODEINFO_H
#define MASTODONT_NODEINFO_H
#include <mastodont_types.h>


struct mstdnt_nodeinfo_metadata
{
    char* version;
};

struct mstdnt_nodeinfo_software
{
    char* name;
    char* repository;
    char* version;
};

struct mstdnt_nodeinfo
{
    struct mstdnt_nodeinfo_software* software;
    struct mstdnt_nodeinfo_metadata* metadata;
    mstdnt_bool open_registrations;
};

int mstdnt_nodeinfo_json(struct mstdnt_nodeinfo* nodeinfo, cJSON* js);

int mstdnt_nodeinfo_json_callback(cJSON* json, void* nodeinfo);

int mastodont_get_nodeinfo(mastodont_t* api,
                           char* version,
                           struct mstdnt_storage* storage,
                           struct mstdnt_nodeinfo* nodeinfo);

void mstdnt_cleanup_nodeinfo(struct mstdnt_nodeinfo* nodeinfo);

#endif /* MASTODONT_NODEINFO_H */
