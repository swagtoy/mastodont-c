/*
 * Licensed under BSD 2-Clause License
 */

#ifndef MASTODONT_H
#define MASTODONT_H
#include <mastodont_hooks.h>
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
#include <mastodont_chats.h>

/// Initializes libcurl
void mstdnt_global_curl_init();
/// Cleans up libcurl
void mstdnt_global_curl_cleanup();

/**
 * Initializes a mstdnt struct
 *
 * @param data Pointer to struct to fill in
 * @return Value of curl_easy_init(); either Zero or non-zero
 */
int mstdnt_init(mastodont_t* data);

/**
 * Cleans up the mstdnt struct
 *
 * @param data Pointer to the mstdnt data
 */
void mstdnt_cleanup(mastodont_t* data);

/**
 * Cleans up a storage struct.
 *
 * This contains information such as JSON information and errors.
 *
 * @param storage The storage block to cleanup
 */
void mstdnt_storage_cleanup(struct mstdnt_storage* storage);

#endif /* MASTODONT_H */
