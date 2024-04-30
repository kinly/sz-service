cmake_minimum_required(VERSION 3.15)
project(sz_service)

include_directories(include)

#the thread library of the system.
find_package(Threads REQUIRED)

include(cmake/build.cmake)
include(cmake/develop.cmake)
include(cmake/find.hiredis.cmake)

add_subdirectory(${sz_service_SOURCE_DIR}/game-service)
