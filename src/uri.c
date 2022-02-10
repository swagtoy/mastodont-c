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

#include <mastodont_uri.h>
#include <string.h>

struct mstdnt_uri mastodont_uristr_to_uri(char* uri, size_t len)
{
    int i;
    struct mstdnt_uri ret;
    if (len <= 0)
        len = strlen(uri);

    ret.user = ret.storage;
    /* Copy information over */
    for (i = 0; i < len; ++i)
    {
        switch (uri[i])
        {
        case '@':
            ret.storage[i] = '\0';
            ret.domain = ret.storage + i + 1;
            break;
        default:
            ret.storage[i] = uri[i];
            break;
        }
    }
    
    ret.storage[i] = '\0';
    return ret;
}
