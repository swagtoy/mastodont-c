cmake_minimum_required(VERSION 3.14)
project(mastodont
  VERSION 0.1
  DESCRIPTION "Library for Mastodon, Pleroma, Wormhole, etc."
  LANGUAGES C)

# Useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
add_library(mastodont STATIC)

set(HEADERS_FILES
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

target_sources(mastodont PUBLIC
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
  src/uri.c
  
  PRIVATE
  libs/cjson/cJSON.c
)

target_compile_options(mastodont PUBLIC
  -Wall -Wextra -std=c99
  -Wshadow -Wcast-align -Wstrict-prototypes
)

target_link_options(mastodont PUBLIC
  -lmastodont
  ${CURL_LDFLAGS_OTHER}
)

target_include_directories(mastodont PUBLIC
  include/
  ${CURL_INCLUDE_DIRS}

  PRIVATE
  libs/
)

install(FILES ${HEADER_FILES} DESTINATION include/mastodont/)
install(TARGETS mastodont LIBRARY DESTINATION lib)
# TODO
#install(FILES MastodontConfig.cmake DESTINATION lib/cmake/mastodont)

