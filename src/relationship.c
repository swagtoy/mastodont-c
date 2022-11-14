/*
 * Licensed under BSD 3-Clause License
 */

#include <stdlib.h>
#include <mastodont_hooks.h>
#include <mastodont_relationship.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>
#include <mastodont_generate.h>

#define FLAG_ARG(flag) { &(relationship->flags), flag }

struct _mstdnt_relationships_cb_args
{
    struct mstdnt_relationship** relationships;
    size_t* size;
};

struct _mstdnt_relationship_flags_args
{
    mstdnt_relationship_flag_t* flags;
    mstdnt_relationship_flag_t flag;
};

static void
_mstdnt_val_relationship_flag_call(cJSON* v, void* _type)
{
    struct _mstdnt_relationship_flags_args* arg = _type;
    if (cJSON_IsTrue(v))
        *(arg->flags) |= arg->flag;
}

int
mstdnt_relationship_json(struct mstdnt_relationship* relationship, cJSON* js)
{
    cJSON* v;

    /* Store values as flags instead of booleans, this saves us
     * a little bit of space but does involve a bit more typing */
    struct _mstdnt_relationship_flags_args args[11] = {
        FLAG_ARG(MSTDNT_RELATIONSHIP_FOLLOWING),
        FLAG_ARG(MSTDNT_RELATIONSHIP_REQUESTED),
        FLAG_ARG(MSTDNT_RELATIONSHIP_ENDORSED),
        FLAG_ARG(MSTDNT_RELATIONSHIP_FOLLOWED_BY),
        FLAG_ARG(MSTDNT_RELATIONSHIP_MUTING),
        FLAG_ARG(MSTDNT_RELATIONSHIP_MUTING_NOTIFS),
        FLAG_ARG(MSTDNT_RELATIONSHIP_SHOWING_REBLOGS),
        FLAG_ARG(MSTDNT_RELATIONSHIP_NOTIFYING),
        FLAG_ARG(MSTDNT_RELATIONSHIP_BLOCKING),
        FLAG_ARG(MSTDNT_RELATIONSHIP_DOMAIN_BLOCKING),
        FLAG_ARG(MSTDNT_RELATIONSHIP_BLOCKED_BY)
    };

    struct _mstdnt_val_ref vals[] = {
        { "id", &(relationship->id), _mstdnt_val_string_call },
        { "following", args, _mstdnt_val_relationship_flag_call },
        { "requested", args + 1, _mstdnt_val_relationship_flag_call },
        { "endorsed", args + 2, _mstdnt_val_relationship_flag_call },
        { "followed_by", args + 3, _mstdnt_val_relationship_flag_call },
        { "muting", args + 4, _mstdnt_val_relationship_flag_call },
        { "muting_notifications", args + 5, _mstdnt_val_relationship_flag_call },
        { "showing_reblogs", args + 6, _mstdnt_val_relationship_flag_call },
        { "notifying", args + 7, _mstdnt_val_relationship_flag_call },
        { "blocking", args + 8, _mstdnt_val_relationship_flag_call },
        { "domain_blocking", args + 9, _mstdnt_val_relationship_flag_call },
        { "blocked_by", args + 10, _mstdnt_val_relationship_flag_call },
        { "note", &(relationship->note), _mstdnt_val_string_call },
    };

    for (v = js; v; v = v->next)
        _mstdnt_key_val_ref(v, vals, _mstdnt_arr_len(vals));

    return 0;
}

// GENERATE mstdnt_statuses_json
GENERATE_JSON_ARRAY_FUNC(mstdnt_relationships_json, struct mstdnt_relationship, mstdnt_relationship_json)

int
mstdnt_relationship_json_callback(cJSON* json,
                                  void* args,
                                  mstdnt_request_cb_data* data)
{
    (void)args;
    mstdnt_relationship* rel = malloc(sizeof(mstdnt_relationship));
    data->data = rel;
    return mstdnt_relationship_json(rel, json);
}

int
mstdnt_relationships_json_callback(cJSON* json,
                                   void* args,
                                   mstdnt_request_cb_data* data)
{
    (void)args;
    mstdnt_relationships* rels = malloc(sizeof(mstdnt_relationship));
    data->data = rels;
    data->data_free_cb = (mstdnt_data_free_cb_t)mstdnt_cleanup_relationships;
    return mstdnt_relationships_json(&(rels->relationships), &(rels->len), json);
}

int
mstdnt_get_relationships(mastodont_t* data,
                         struct mstdnt_args* m_args,
                         mstdnt_request_cb_t cb_request,
                         void* cb_args,
                         char** ids,
                         size_t ids_len)
{
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_ARRAY, "id",
          {
                .a.arr = ids,
                .a.arr_len = ids_len
          }
        }
    };
    
    struct mstdnt_request_args req_args = {
        .url = "api/v1/accounts/relationships",
        .params_query = params,
        .params_query_len = _mstdnt_arr_len(params),
        .request_type = CURLOPT_HTTPGET,
        .callback = mstdnt_relationships_json_callback
    };

    return mstdnt_request(data, m_args, cb_request, cb_args, &req_args);
}

void
mstdnt_cleanup_relationships(struct mstdnt_relationships* rels)
{
    if (!rels) return;
    mstdnt_free(rels->relationships);
}
