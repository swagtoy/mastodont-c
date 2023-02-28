#include <stdio.h>
#include <mastodont.h>

char* get_line(char const* prompt, size_t* res_size)
{
	char* result = NULL;
#define GL_BUF_SIZE 25
	size_t _res_size;
	char buffer[GL_BUF_SIZE];
	
	if (prompt)
	{
		fputs(prompt, stdout);
		fputs(": ", stdout);
	}
	
	while (fgets(buffer, GL_BUF_SIZE, stdin) != NULL)
	{
		_res_size += GL_BUF_SIZE;
		result = realloc(result, _res_size);
		memcpy(result, buffer, GL_BUF_SIZE);
	}
	
	if (res_size)
		*res_size = _res_size;
		
	return result;
}

int
main()
{
	mstdnt_global_curl_init();
	
	mastodont_t data;
	
	if (mstdnt_init(&data) != 0)
	{
		fputs("Couldn't initialize Mastodont-c!", stderr);
	}

	char* instance = get_line("Instance: ", NULL);
	
	puts(instance);
	
	free(instance);
	
	mstdnt_cleanup(&data);
	mstdnt_global_curl_cleanup();
}