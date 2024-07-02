cmake_minimum_required(VERSION 3.15)
project(sz_service)

if (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
endif()
SET(CMAKE_DEBUG_POSTFIX d)

SET(ROOT_PROJECT_SOURCE_DIR ${sz_service_SOURCE_DIR})
message(${ROOT_PROJECT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_module)

#the thread library of the system.
find_package(Threads REQUIRED)

include(cmake/build.cmake)
include(cmake/develop.cmake)
# include(cmake/find.hiredis.cmake)


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(${sz_service_SOURCE_DIR}/game-service)
add_subdirectory(${sz_service_SOURCE_DIR}/util)
