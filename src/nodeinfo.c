/*
 * Licensed under BSD 3-Clause License
 */

#include <stdlib.h>
#include <string.h>
#include <mastodont_nodeinfo.h>
#include <mastodont_hooks.h>
#include <mastodont_request.h>
#include <mastodont_json_helper.h>

static void
_mstdnt_val_software_malloc_call(cJSON* v, void* _type)
{
    struct mstdnt_nodeinfo_software** type = _type;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_nodeinfo_software));

    // Do json stuff in here, it's only done once
    if (*type)
    {
        struct mstdnt_nodeinfo_software* soft = *type;

        struct _mstdnt_val_ref refs[] = {
            { "name", &(soft->name), _mstdnt_val_string_call },
            { "repository", &(soft->repository), _mstdnt_val_string_call },
            { "version", &(soft->version), _mstdnt_val_string_call },
        };

        for (cJSON* w = v->child; w; w = w->next)
            _mstdnt_key_val_ref(w, refs, _mstdnt_arr_len(refs));
    }
}


static void
_mstdnt_val_metadata_malloc_call(cJSON* v, void* _type)
{
    struct mstdnt_nodeinfo_metadata** type = _type;

    *type = mstdnt_calloc(1, sizeof(struct mstdnt_nodeinfo_metadata));

    // Do json stuff in here, it's only done once
    if (*type)
    {
        struct mstdnt_nodeinfo_metadata* soft = *type;

        struct _mstdnt_val_ref refs[] = {
            // TODO
            { "nodeName", &(soft->node_name), _mstdnt_val_string_call },
            { "nodeDescription", &(soft->node_description), _mstdnt_val_string_call },
        };

        for (cJSON* w = v->child; w; w = w->next)
            _mstdnt_key_val_ref(w, refs, _mstdnt_arr_len(refs));
    }
}

int
mstdnt_nodeinfo_json(struct mstdnt_nodeinfo* nodeinfo, cJSON* js)
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

int
mstdnt_nodeinfo_json_callback(cJSON *json,
                              void  *args,
                              mstdnt_request_cb_data *data)
{
	(void)args;
	struct mstdnt_nodeinfo* ninfo = malloc(sizeof(struct mstdnt_nodeinfo));
	data->data = ninfo;
	data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_nodeinfo;
    return mstdnt_nodeinfo_json(ninfo, json->child);
}

int mstdnt_get_nodeinfo(mastodont_t* api,
                        struct mstdnt_args* m_args,
                        mstdnt_request_cb_t cb_request,
                        void* cb_args,
                        char* version)
{
    char url[MSTDNT_URLSIZE];
    snprintf(url, MSTDNT_URLSIZE, "nodeinfo/%s.json", version ? version : "2.1");
    
	struct mstdnt_request_args req_args = {
		.url = url,
		.request_type = CURLOPT_HTTPGET,
		.callback = mstdnt_nodeinfo_json_callback,
	};

    return mstdnt_request(api, m_args, cb_request, cb_args, &req_args);
}

void mstdnt_cleanup_nodeinfo(struct mstdnt_nodeinfo* nodeinfo)
{
    if (!nodeinfo) return;
    mstdnt_free(nodeinfo->software);
    mstdnt_free(nodeinfo->metadata);
}

