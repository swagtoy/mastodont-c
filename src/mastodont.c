#include <string.h>
#include <stdlib.h>
#include <mastodont.h>
#include <curl/curl.h>

void mastodont_global_curl_init()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void mastodont_global_curl_cleanup()
{
    curl_global_cleanup();
}

// Curl multi can still be used with single context's
int mastodont_init(mastodont_t* data)
{
    data->curl = curl_multi_init();
    return data->curl == NULL;
}

void mastodont_cleanup(mastodont_t* data)
{
    curl_multi_cleanup(data->curl);
}

void mastodont_free(void* ptr)
{
    free(ptr);
}

void mastodont_storage_cleanup(struct mstdnt_storage* storage)
{
    if (storage->needs_cleanup)
    {
        cJSON_Delete(storage->root);
        storage->needs_cleanup = 0;
    }
}
