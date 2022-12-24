/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_RELATIONSHIP_H
#define MASTODONT_RELATIONSHIP_H
#include <stdint.h>
#include <cjson/cJSON.h>
#include "mastodont_types.h"
#include "mastodont_request.h"

typedef uint16_t mstdnt_relationship_flag_t;

#define MSTDNT_RELATIONSHIP_NOOP 0
#define MSTDNT_RELATIONSHIP_FOLLOWING (1<<0)
#define MSTDNT_RELATIONSHIP_REQUESTED (1<<1)
#define MSTDNT_RELATIONSHIP_ENDORSED (1<<2)
#define MSTDNT_RELATIONSHIP_FOLLOWED_BY (1<<3)
#define MSTDNT_RELATIONSHIP_MUTING (1<<4)
#define MSTDNT_RELATIONSHIP_MUTING_NOTIFS (1<<5)
#define MSTDNT_RELATIONSHIP_SHOWING_REBLOGS (1<<6)
#define MSTDNT_RELATIONSHIP_NOTIFYING (1<<7)
#define MSTDNT_RELATIONSHIP_BLOCKING (1<<8)
#define MSTDNT_RELATIONSHIP_DOMAIN_BLOCKING (1<<9)
#define MSTDNT_RELATIONSHIP_BLOCKED_BY (1<<10)

typedef struct mstdnt_relationship
{
    char* id;
    mstdnt_relationship_flag_t flags;
    char* note;
} mstdnt_relationship;

typedef struct mstdnt_relationships
{
    mstdnt_relationship* relationships;
    size_t len;
} mstdnt_relationships;

int
mstdnt_relationship_json_callback(cJSON* json,
                                  void* args,
                                  mstdnt_request_cb_data* data);

int mstdnt_relationship_json(struct mstdnt_relationship* relationship, cJSON* js);

int mstdnt_relationships_json(struct mstdnt_relationship* relationships[],
                              size_t* size,
                              cJSON* json);

int
mstdnt_relationships_json_callback(cJSON* json,
                                   void* args,
                                   mstdnt_request_cb_data* data);

int
mstdnt_get_relationships(mastodont_t* data,
                         struct mstdnt_args* args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char** ids,
                         size_t ids_len);

void mstdnt_cleanup_relationships(struct mstdnt_relationships* rels);


#endif /* MASTODONT_RELATIONSHIP_H */
