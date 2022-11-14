/*
 * Licensed under BSD 3-Clause License
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
