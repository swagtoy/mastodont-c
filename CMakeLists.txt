cmake_minimum_required(VERSION 3.14)
project(mastodont-c
  VERSION 0.0
  DESCRIPTION "Terrible library for Mastodon and Pleroma 'n Friends"
  LANGUAGES C)

# Useful
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
add_library(mastodont-c STATIC)

target_sources(mastodont-c PUBLIC
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

target_compile_options(mastodont-c PUBLIC
  -Wall -Wextra -std=c99
  -Wshadow -Wcast-align -Wstrict-prototypes
)

target_link_options(mastodont-c PUBLIC
  -lmastodont
  ${CURL_LDFLAGS_OTHER}
)

target_include_directories(mastodont-c PUBLIC
  include/
  ${CURL_INCLUDE_DIRS}

  PRIVATE
  libs/
)

