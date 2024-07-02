set(project_name util)
project(${project_name})
SET(CMAKE_DEBUG_POSTFIX d)

include_directories(./)

# files
include(./cmake/files.cmake)

add_custom_target(${project_name} SOURCES ${ALL_FILES})
