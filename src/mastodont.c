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
    return data->curl == NULL;
}

void mastodont_free(mastodont_t* data)
{
    curl_easy_cleanup(data->curl);
}
