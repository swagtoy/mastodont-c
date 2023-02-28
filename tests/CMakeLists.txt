add_executable(get_feed get_feed.c)
target_compile_options(get_feed PUBLIC -std=c99 -Wall)
target_include_directories(get_feed PUBLIC
	../include
	${CURL_INCLUDE_DIRS}
	../libs)