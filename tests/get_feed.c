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
	result = realloc(result, strlen(buffer)+1);
	strcpy(result, buffer);
	
	return result;
}

int
main(int argc, char** argv)
{
	mstdnt_global_curl_init();
	
	mastodont_t data;
	
	if (mstdnt_init(&data) != 0)
	{
		fputs("Couldn't initialize Mastodont-c!", stderr);
	}

	char* instance = get_line("Instance");
	
	puts(instance);
	
	free(instance);
	
	mstdnt_cleanup(&data);
	mstdnt_global_curl_cleanup();
	
	return 1;
}