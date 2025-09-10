cmake_minimum_required(VERSION 3.14)
project(mastodont
  VERSION 0.1
  DESCRIPTION "Library for Mastodon, Pleroma, Wormhole, etc."
  LANGUAGES C CXX)

# BEGIN cmake package config info
include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include/mastodont/)
set(LIB_INSTALL_DIR lib/)
# This feels... UNIX-y
set(LIB_INSTALL_PATH lib/libmastodont.a)
# END cmake package config info

# Useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_CXX_BINDINGS "Build the C++ bindings (mastodont-cxx)" ON)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
# Find the real cJSON
pkg_check_modules(CJSON libcjson)

if (NOT (CJSON_FOUND AND CJSON_VERSION VERSION_GREATER_EQUAL 1.7.14))
  # This was a workaround to some distributions providing an older cJSON
  message("CJSON not found or CJSON_VERSION < 1.7.14, falling back to header-only.")
  list(APPEND SRC_FILES libs/cjson/cJSON.c)
endif()

set(HEADER_FILES
  include/mastodont_account.h
  include/mastodont_announcement.h
  include/mastodont_application.h
  include/mastodont_attachment.h
  include/mastodont_chats.h
  include/mastodont_codes.h
  include/mastodont_emoji.h
  include/mastodont_error.h
  include/mastodont_fetch.h
  include/mastodont_generate.h
  include/mastodont_history.h
  include/mastodont_hooks.h
  include/mastodont_instance.h
  include/mastodont_json_helper.h
  include/mastodont_list.h
  include/mastodont_mention.h
  include/mastodont_nodeinfo.h
  include/mastodont_notif_types.h
  include/mastodont_notification.h
  include/mastodont_pleroma.h
  include/mastodont_query.h
  include/mastodont_relationship.h
  include/mastodont_request.h
  include/mastodont_scrobbles.h
  include/mastodont_search.h
  include/mastodont_status.h
  include/mastodont_tag.h
  include/mastodont_timeline.h
  include/mastodont_types.h
  include/mastodont_uri.h
  include/mastodont_visibility_types.h
  include/mastodont.h
)

list(APPEND SRC_FILES
  src/account.c
  src/application.c
  src/attachment.c
  src/chats.c
  src/emoji.c
  src/error.c
  src/fetch.c
  src/history.c
  src/hooks.c
  src/instance.c
  src/json_helper.c
  src/list.c
  src/mastodont.c
  src/nodeinfo.c
  src/notification.c
  src/pleroma.c
  src/query.c
  src/relationship.c
  src/request.c
  src/scrobbles.c
  src/search.c
  src/status.c
  src/tag.c
  src/timeline.c
  src/uri.c)

add_library(mastodont STATIC ${HEADER_FILES})
target_sources(mastodont PUBLIC ${SRC_FILES})

target_compile_options(mastodont PUBLIC
  -Wall -Wextra -std=c99
  -Wshadow -Wcast-align -Wstrict-prototypes
)

target_link_options(mastodont PUBLIC
  ${CURL_LDFLAGS_OTHER}
)

target_include_directories(mastodont PUBLIC
  include/
  ${CURL_INCLUDE_DIRS}
  PRIVATE
  libs/
)

add_subdirectory(bindings)
add_subdirectory(tests)

# Install
if(CJSON_FOUND AND CJSON_VERSION VERSION_GREATER_EQUAL 1.7.14)
  install(FILES ${HEADER_FILES} DESTINATION
    include/mastodont/)
  install(TARGETS mastodont
    LIBRARY DESTINATION lib)

  configure_package_config_file(cmake/MastodontConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mastodont-config.cmake
    INSTALL_DESTINATION lib/cmake/mastodont
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR LIB_INSTALL_PATH)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mastodont-config.cmake DESTINATION lib/cmake/mastodont)
else()
  message("Since CJSON wasn't found or your distribution doesn't provide correct version, you can still use cmake/mastodont-config.cmake and check for it in your package. You _probably_ don't want this")
endif()
