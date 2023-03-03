#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastodont.h>

char* get_line(char const* prompt)
{
	char* result = NULL;
#define GL_BUF_SIZE 255
	char buffer[GL_BUF_SIZE];
	
	if (prompt)
	{
		fputs(prompt, stdout);
		fputs(": ", stdout);
	}
	
	fgets(buffer, GL_BUF_SIZE, stdin);
	int buffer_len = strlen(buffer);
	result = realloc(result, buffer_len+1);
	memcpy(result, buffer, buffer_len);
	result[buffer_len-1] = '\0';
	
	return result;
}

int
tl_callback(mstdnt_request_cb_data* cb_data, void* args)
{
	struct mstdnt_statuses* statuses = MSTDNT_CB_DATA(cb_data);
	
	for (int i = 0; i < statuses->len; ++i)
	{
		struct mstdnt_status* status = statuses->statuses + i;
		puts("---------------- BEGIN STATUS ----------------");
		printf(" Author: %s", status->account.username);
		puts("----------------- END STATUS -----------------");
	}

	return MSTDNT_REQUEST_DONE;
}

int
main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	mstdnt_global_curl_init();
	
	mastodont_t data;
	
	if (mstdnt_init(&data) != 0)
	{
		fputs("Couldn't initialize Mastodont-c!", stderr);
		exit(1);
	}

	char* instance = get_line("Instance");
	
	struct mstdnt_args m_args = {
		.url = instance,
		.token = NULL,
		.flags = 0,
	};
	
	mstdnt_timeline_public(&data, &m_args, tl_callback, NULL, (struct mstdnt_timeline_args){});
	
	mstdnt_await(&data, 0, NULL, 0);

	// Cleanup	
	free(instance);
	mstdnt_cleanup(&data);
	mstdnt_global_curl_cleanup();
	
	return 1;
}