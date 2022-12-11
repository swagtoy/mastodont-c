# Get CFLAGS
set(MASTODONT_FOUND)
set(MASTODONT_VERSION 0.1)

# Get parent directory of this config file
find_package(CURL REQUIRED)

set(MASTODONT_INCLUDE_DIRS
  "${CMAKE_CURRENT_LIST_DIR}/../include"
  "${CMAKE_CURRENT_LIST_DIR}/../libs"
)
set(MASTODONT_LIBRARIES "mastodont" "curl")


