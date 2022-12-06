cmake_minimum_required(VERSION 3.14)
project(mastodont-c
  VERSION 0.0
  DESCRIPTION "Terrible library for Mastodon and Pleroma 'n Friends"
  LANGUAGES C)

find_package(CURL REQUIRED)
add_library(mastodont-c STATIC)

target_sources(mastodont-c PUBLIC
  account.c
  application.c
  attachment.c
  chats.c
  emoji.c
  error.c
  fetch.c
  history.c
  hooks.c
  instance.c
  json_helper.c
  list.c
  mastodont.c
  nodeinfo.c
  notification.c
  pleroma.c
  query.c
  relationship.c
  request.c
  scrobbles.c
  search.c
  status.c
  tag.c
  timeline.c
  uri.c
)

target_compile_options(mastodont-c PUBLIC
  -Wall -Wextra -std=c99
  -Wshadow -Wcast-align -Wstrict-prototypes
  -Werror=implicit-function-decleration
)

target_link_options(mastodont-c PUBLIC
  -lmastodont
  ${CURL_LDFLAGS_OTHER}
)

target_include_directories(mastodont-c PUBLIC include/)

