/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_NODEINFO_H
#define MASTODONT_NODEINFO_H
#include "mastodont_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mstdnt_nodeinfo_metadata
{
    char* node_name;
    char* node_description;
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

int mstdnt_nodeinfo_json_callback(cJSON* json, void* args, mstdnt_request_cb_data* result);

int mstdnt_get_nodeinfo(mastodont_t* api,
                        struct mstdnt_args* args,
                        mstdnt_request_cb_t cb_request,
                        void* cb_args,
                        char* version);

void mstdnt_cleanup_nodeinfo(struct mstdnt_nodeinfo* nodeinfo);

#ifdef __cplusplus
}
#endif

#endif /* MASTODONT_NODEINFO_H */
