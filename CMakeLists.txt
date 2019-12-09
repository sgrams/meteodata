##
## meteodata-git
## Stanislaw Grams <sjg@fmdx.pl>
##
## CMakeLists.txt
## See LICENSE for license information
##
cmake_minimum_required (VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
project (meteodata)

# pkg-config
find_package (PkgConfig REQUIRED)

# libcurl
pkg_check_modules(LIBCURL REQUIRED libcurl)
include_directories(${LIBCURL_INCLUDE_DIRS})
link_directories(${LIBCURL_LIBRARY_DIRS})
add_definitions(${LIBCURL_CFLAGS_OTHER})

## build flags
set (CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wall -pedantic"
  )

## src
set (SRC_FILES
  # *.cc, *.c
  src/agent.cc
  src/commandparser.cc
  src/data.cc
  src/main.cc
  src/station.cc

  # *.hh, *.h
  include/agent.hh
  include/commandparser.hh
  include/data.hh
  include/helper.hh
  include/station.hh
  include/types.hh
  )

## lib
set (LIBS
  ${LIBCURL_LIBRARIES}
  )

## executable
add_executable (
  meteodata
  ${SRC_FILES}
  )

target_link_libraries (
  meteodata
  ${LIBS})
