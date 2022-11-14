/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_NOTIF_TYPES
#define MASTODONT_NOTIF_TYPES
#include <stdint.h>

/* uint16 */
#define MSTDNT_NOTIFICATION_NOOP 0
#define MSTDNT_NOTIFICATION_FOLLOW (1<<0)
#define MSTDNT_NOTIFICATION_FOLLOW_REQUEST (1<<1)
#define MSTDNT_NOTIFICATION_MENTION (1<<2)
#define MSTDNT_NOTIFICATION_REBLOG (1<<3)
#define MSTDNT_NOTIFICATION_FAVOURITE (1<<4)
#define MSTDNT_NOTIFICATION_POLL (1<<5)
#define MSTDNT_NOTIFICATION_STATUS (1<<6)
#define MSTDNT_NOTIFICATION_EMOJI_REACT (1<<7)
#define MSTDNT_NOTIFICATION_CHAT_MENTION (1<<8)
#define MSTDNT_NOTIFICATION_REPORT (1<<9)

typedef uint16_t mstdnt_notification_t;

/**
 * Turns a notification type into a string
 *
 * @param type Notification type.
 * @return String literal representing type, e.g. 'follow', 'follow request'
 */
const char* mstdnt_notification_t_to_str(mstdnt_notification_t type);

#endif /* MASTODONT_NOTIF_TYPES */
