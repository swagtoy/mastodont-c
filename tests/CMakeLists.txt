include(../cmake/mastodont-config.cmake)
add_executable(get_feed get_feed.c)
target_compile_options(get_feed PUBLIC -std=c99 -Wall)
target_link_libraries(get_feed PUBLIC
	${MASTODONT_LIBRARIES}
)
target_include_directories(get_feed PUBLIC
	${MASTODONT_INCLUDE_DIRS}
)
	