list(APPEND CXX_SRC_FILES
	mastodont.cpp)

set(CXX_HEADER_FILES
	include/mastodont.hpp)
	
add_library(mastodont-cxx STATIC ${CXX_HEADER_FILES})
target_sources(mastodont-cxx PUBLIC ${CXX_SRC_FILES})

target_include_directories(mastodont-cxx PUBLIC
	include/
	${PROJECT_SOURCE_DIR}/include
	PRIVATE
	mastodont)

target_compile_options(mastodont-cxx PUBLIC
	-Wall -Wextra -std=c++17)

set(INCLUDE_INSTALL_DIR include/mastodont-cxx/)
set(LIB_INSTALL_DIR lib/)
# This feels... UNIX-y
set(LIB_INSTALL_PATH lib/libmastodont-cxx.a)

	
install(FILES ${CXX_HEADER_FILES} DESTINATION
	include/mastodont-cxx/)
install(TARGETS mastodont-cxx
	LIBRARY DESTINATION lib)

#configure_package_config_file(cmake/MastodontConfig.cmake.in
#	${CMAKE_CURRENT_BINARY_DIR}/mastodont-config.cmake
#	INSTALL_DESTINATION lib/cmake/mastodont
#	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR LIB_INSTALL_PATH)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mastodont-config.cmake DESTINATION lib/cmake/mastodont)
