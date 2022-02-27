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

#include <mastodont_query.h>
#include <mastodont_json_helper.h>

int mstdnt_check_error(struct mstdnt_fetch_results* results,
                       struct mstdnt_storage* storage)
{
    int res = 0;
    cJSON* root, *v;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    struct _mstdnt_val_ref refs[] = {
        { "error", &(storage->error), _mstdnt_val_string_call },
        { "error_description", &(storage->error_description), _mstdnt_val_string_call },
    };
    
    for (v = root->child; v; v = v->next)
        if (_mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs)) == 0)
            res = 1;
    return res;
}
