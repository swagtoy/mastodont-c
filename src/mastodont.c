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

int mastodont_init(mastodont_t* data)
{
    data->curl = curl_easy_init();
    data->token = NULL;
    data->token_heap = 0;
    return data->curl == NULL;
}

int mastodont_set_token(mastodont_t* data, char* token)
{
    char* mtoken = malloc(strlen(token)+1);
    if (!mtoken)
        return 1;
    data->token_heap = 1;
    strcpy(mtoken, token);
    data->token = mtoken;
    return 0;
}

void mastodont_free(mastodont_t* data)
{
    curl_easy_cleanup(data->curl);
    if (data->token_heap)
        free(data->token);
}

void mastodont_storage_cleanup(struct mstdnt_storage* storage)
{
    if (storage->needs_cleanup)
    {
        cJSON_Delete(storage->root);
        storage->needs_cleanup = 0;
    }
}
