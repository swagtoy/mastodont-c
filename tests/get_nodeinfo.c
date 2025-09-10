#include "mastodont_nodeinfo.h"
#include "mastodont_types.h"
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

static char const*
itob(int i)
{
	return i ? "true" : "false";
}

int
ni_callback(mstdnt_request_cb_data* cb_data, void* args)
{
	struct mstdnt_nodeinfo* ninfo = MSTDNT_CB_DATA(cb_data);
	if (!ninfo)
	{
		printf("Failed to make the request...\n -- %s\n", cb_data->storage.error);
		return MSTDNT_REQUEST_DONE;
	}
	
	if (ninfo->software)
	printf(
		"Software:\n"
		"   Name: %s\n"
		"   Repository: %s\n"
		"   Version: %s\n",
		ninfo->software->name,
		ninfo->software->repository,
		ninfo->software->version
	);
	
	
	if (ninfo->metadata)
	printf(
		"Metadata:\n"
		"   Name: %s\n"
		"   Descriptions: %s\n",
		ninfo->metadata->node_name,
		ninfo->metadata->node_description
	);
	
	printf(
		"Open registrations? %s\n",
		ninfo->open_registrations ? "True" : "False"
	);

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
	
	
	printf("==== Nodeinfo for %s ====\n", instance);
	mstdnt_get_nodeinfo(&data, &m_args, ni_callback, NULL, NULL);
	
	mstdnt_await(&data, 0, NULL, 0);

	// Cleanup	
	free(instance);
	mstdnt_cleanup(&data);
	mstdnt_global_curl_cleanup();
	
	return 1;
}
