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

#include <stdlib.h>
#include <mastodont_relationship.h>
#include <mastodont_json_helper.h>
#include <mastodont_query.h>

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

static void _mstdnt_val_relationship_flag_call(cJSON* v, void* _type)
{
    struct _mstdnt_relationship_flags_args* arg = _type;
    if (cJSON_IsTrue(v))
        *(arg->flags) |= arg->flag;
}

int mstdnt_relationship_json(struct mstdnt_relationship* relationship, cJSON* js)
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

int mstdnt_relationships_result(struct mstdnt_fetch_results* results,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships[],
                                size_t* size)
{
    size_t i = 0;
    cJSON* root, *rel_j_list;
    if (_mstdnt_json_init(&root, results, storage) &&
        !cJSON_IsArray(root))
        return 1;

    if (size) *size = cJSON_GetArraySize(root);

    *relationships = calloc(1, (size ? *size : cJSON_GetArraySize(root))
                       * sizeof(struct mstdnt_relationship));
    if (*relationships == NULL)
        return 1;

    cJSON_ArrayForEach(rel_j_list, root)
    {
        mstdnt_relationship_json((*relationships) + i++, rel_j_list->child);
    }

    return 0;
}

int mstdnt_relationship_result(struct mstdnt_fetch_results* results,
                               struct mstdnt_storage* storage,
                               struct mstdnt_relationship* relationship)
{
    if (!relationship) return 0;

    cJSON* root;
    if (_mstdnt_json_init(&root, results, storage))
        return 1;

    return mstdnt_relationship_json(relationship, root->child);
}

int _mstdnt_relationship_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args)
{
    return mstdnt_relationship_result(results, storage, (struct mstdnt_relationship*)_args);
}

int _mstdnt_relationships_result_callback(struct mstdnt_fetch_results* results,
                                          struct mstdnt_storage* storage,
                                          void* _args)
{
    struct _mstdnt_relationships_cb_args* args = _args;
    return mstdnt_relationships_result(results, storage, args->relationships, args->size);
}

int mastodont_get_relationships(mastodont_t* data,
                                char** ids,
                                size_t ids_len,
                                struct mstdnt_storage* storage,
                                struct mstdnt_relationship* relationships[],
                                size_t* size)
{
    struct _mstdnt_relationships_cb_args cb_args = { relationships, size };
    struct _mstdnt_query_param params[] = {
        { _MSTDNT_QUERY_ARRAY, "id",
          {
                .a.arr = ids,
                .a.arr_len = ids_len
          }
        }
    };
    
    struct mastodont_request_args req_args = {
        storage,
        "api/v1/accounts/relationships",
        params, _mstdnt_arr_len(params),
        NULL, 0,
        CURLOPT_HTTPGET,
        &cb_args,
        _mstdnt_relationships_result_callback
    };

    return mastodont_request(data, &req_args);
}

void mstdnt_cleanup_relationships(struct mstdnt_relationship* rels)
{
    if (!rels) return;
    free(rels);
}
