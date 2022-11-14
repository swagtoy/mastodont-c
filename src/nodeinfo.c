/*
 * Licensed under BSD 2-Clause License
 */

#include <stdlib.h>
#include <string.h>
#include <mastodont_nodeinfo.h>
#include <mastodont_hooks.h>
#include <mastodont_request.h>
#include <mastodont_json_helper.h>

static void _mstdnt_val_software_malloc_call(cJSON* v, void* _type)
{
    struct mstdnt_nodeinfo_software** type = _type;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_nodeinfo_software));

    // Do json stuff in here, it's only done once
    if (*type)
    {
        struct mstdnt_nodeinfo_software* soft = *type;

        // Zero
        memset(soft, 0, sizeof(struct mstdnt_nodeinfo_software));

        struct _mstdnt_val_ref refs[] = {
            { "name", &(soft->name), _mstdnt_val_string_call },
            { "repository", &(soft->repository), _mstdnt_val_string_call },
            { "version", &(soft->version), _mstdnt_val_string_call },
        };

        for (cJSON* w = v->child; w; w = w->next)
            _mstdnt_key_val_ref(w, refs, _mstdnt_arr_len(refs));
    }
}


static void _mstdnt_val_metadata_malloc_call(cJSON* v, void* _type)
{
    struct mstdnt_nodeinfo_metadata** type = _type;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_nodeinfo_metadata));

    // Do json stuff in here, it's only done once
    if (*type)
    {
        struct mstdnt_nodeinfo_metadata* soft = *type;

        // Zero
        memset(soft, 0, sizeof(struct mstdnt_nodeinfo_metadata));

        struct _mstdnt_val_ref refs[] = {
            // TODO
            { "version", &(soft->version), _mstdnt_val_string_call },
        };

        for (cJSON* w = v->child; w; w = w->next)
            _mstdnt_key_val_ref(w, refs, _mstdnt_arr_len(refs));
    }
}

int mstdnt_nodeinfo_json(struct mstdnt_nodeinfo* nodeinfo, cJSON* js)
{
    // Zero out
    memset(nodeinfo, 0, sizeof(struct mstdnt_nodeinfo));

    struct _mstdnt_val_ref refs[] = {
        { "software", &(nodeinfo->software), _mstdnt_val_software_malloc_call },
        { "metadata", &(nodeinfo->metadata), _mstdnt_val_metadata_malloc_call },
        { "openRegistrations", &(nodeinfo->open_registrations), _mstdnt_val_bool_call },
        // TODO
    };

    for (cJSON* v = js; v; v = v->next)
    {
        _mstdnt_key_val_ref(v, refs, _mstdnt_arr_len(refs));
    }

    return 0;
}

int mstdnt_nodeinfo_json_callback(cJSON* json, void* nodeinfo)
{
    return mstdnt_nodeinfo_json(nodeinfo, json);
}

int mstdnt_get_nodeinfo(mastodont_t* api,
                           struct mstdnt_args* m_args,
mstdnt_request_cb_t cb_request,
void* cb_args,
                           char* version,
                           struct mstdnt_storage* storage,
                           struct mstdnt_nodeinfo* nodeinfo)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "nodeinfo/%s.json", version ? version : "2.1");
    
    struct mstdnt_request_args req_args = {
        storage,
        url,
        NULL, 0,
        NULL, 0,
        CURLOPT_HTTPGET,
        NULL,
        nodeinfo,
        mstdnt_nodeinfo_json_callback
    };

    return mstdnt_request(api, m_args, &req_args, cb_request, cb_args);
}

void mstdnt_cleanup_nodeinfo(struct mstdnt_nodeinfo* nodeinfo)
{
    if (!nodeinfo) return;
    mstdnt_free(nodeinfo->software);
    mstdnt_free(nodeinfo->metadata);
}
