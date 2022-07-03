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

#ifndef MASTODONT_H
#define MASTODONT_H
#include <mastodont_types.h>
#include <mastodont_timeline.h>
#include <mastodont_list.h>
#include <mastodont_notification.h>
#include <mastodont_status.h>
#include <mastodont_relationship.h>
#include <mastodont_account.h>
#include <mastodont_attachment.h>
#include <mastodont_scrobbles.h>
#include <mastodont_search.h>
#include <mastodont_announcement.h>
#include <mastodont_static.h>
#include <mastodont_chats.h>

//! Initializes libcurl
void mastodont_global_curl_init();
//! Cleans up libcurl
void mastodont_global_curl_cleanup();

/*!
 * Initializes a mastodont struct
 *
 * \param data Pointer to struct to fill in
 * \return Value of curl_easy_init(); either Zero or non-zero
 */
int mastodont_init(mastodont_t* data);

/*!
 * Cleans up the mastodont struct
 *
 * \param data Pointer to the mastodont data
 */
void mastodont_cleanup(mastodont_t* data);

//! Call's free() or a library derivative
void mastodont_free(void*);

/*!
 * Cleans up a storage struct.
 *
 * This contains information such as JSON information and errors.
 *
 * \param storage The storage block to cleanup
 */
void mastodont_storage_cleanup(struct mstdnt_storage* storage);

#endif /* MASTODONT_H */
