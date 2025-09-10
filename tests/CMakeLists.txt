include(../cmake/mastodont-config.cmake)

add_executable(get_feed get_feed.c)
target_compile_options(get_feed PUBLIC -std=c99 -Wall)
target_link_libraries(get_feed PUBLIC
	${MASTODONT_LIBRARIES}
)
target_include_directories(get_feed PUBLIC
	${MASTODONT_INCLUDE_DIRS}
)

# GUI demo
pkg_check_modules(EFL_ECORE efl-core)
if (EFL_ECORE_FOUND)
	# EFL is clearly installed...
	pkg_check_modules(EFL_ELEMENTARY elementary)
	
	add_executable(get_feed_efl get_feed_efl.c)
	target_compile_options(get_feed_efl PUBLIC -std=c99 -Wall)
	target_link_libraries(get_feed_efl PUBLIC
		${MASTODONT_LIBRARIES}
		${EFL_ECORE_LIBRARIES}
		${EFL_ELEMENTARY_LIBRARIES}
	)
	target_include_directories(get_feed_efl PUBLIC
		${MASTODONT_INCLUDE_DIRS}
		${EFL_ECORE_INCLUDE_DIRS}
		${EFL_ELEMENTARY_INCLUDE_DIRS}
	)
endif(EFL_ECORE_FOUND)		

add_executable(get_nodeinfo get_nodeinfo.c)
target_compile_options(get_nodeinfo PUBLIC -std=c99 -Wall)
target_link_libraries(get_nodeinfo PUBLIC
	${MASTODONT_LIBRARIES}
)
target_include_directories(get_nodeinfo PUBLIC
	${MASTODONT_INCLUDE_DIRS}

)
