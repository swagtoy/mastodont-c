#include <string.h>
#include <stdlib.h>
#include <mastodont.h>
#include <curl/curl.h>

void
mstdnt_global_curl_init(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void
mstdnt_global_curl_cleanup(void)
{
    curl_global_cleanup();
}

// Curl multi can still be used with single context's
int
mstdnt_init(mastodont_t* data)
{
    data->curl = curl_multi_init();
    return data->curl == NULL;
}

void
mstdnt_cleanup(mastodont_t* data)
{
    curl_multi_cleanup(data->curl);
}

void
mstdnt_storage_cleanup(struct mstdnt_storage* storage)
{
    if (storage && storage->needs_cleanup)
    {
        cJSON_Delete(storage->root);
        storage->needs_cleanup = 0;
    }
}
