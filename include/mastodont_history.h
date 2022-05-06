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

#ifndef MASTODONT_HISTORY
#define MASTODONT_HISTORY
#include <cjson/cJSON.h>
#include <time.h>

struct mstdnt_history
{
    time_t day;
    unsigned uses;
    unsigned accounts;
};

int mstdnt_history_json(struct mstdnt_history* tag, cJSON* js);
int mstdnt_histories_json(struct mstdnt_history* tags[], size_t* array_size, cJSON* js);
void _mstdnt_val_histories_call(cJSON* v, void* _type);

#endif /* MASTODONT_TAG */
