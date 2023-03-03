# Get CFLAGS
set(MASTODONT_FOUND)
set(MASTODONT_VERSION 0.1)

# Get parent directory of this config file
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
# TODO account for built-in cjson lib
pkg_check_modules(MSTDNT_CJSON libcjson)

set(MASTODONT_INCLUDE_DIRS
  ../include
  ../libs
)

set(MASTODONT_LIBRARIES 
	mastodont
	${MSTDNT_CJSON_LIBRARIES}
	${CURL_LIBRARIES}
)

