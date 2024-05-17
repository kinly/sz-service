set(project_name game-service)
project(${project_name})
SET(CMAKE_DEBUG_POSTFIX d)

include(../cmake/3rd.cinatra.cmake)
# include(../cmake/3rd.hiredis.cmake)

include_directories(include)
# include_directories(../3rd/cinatra/include)
include_directories(../3rd/json/include)

include_directories(../3rd/tomlplusplus/include)

file(GLOB header_files *.h *.hpp *.inl)
file(GLOB source_files *.cpp *.c)

source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})

# redis++
find_package(hiredis REQUIRED)
message("${HIREDIS_LIBRARY_DEBUG}")
message("${HIREDIS_LIBRARY_RELEASE}")
message("${HIREDIS_INCLUDE_DIR}")
include_directories(${HIREDIS_INCLUDE_DIR})
include_directories(${HIREDIS_INCLUDE_DIR}/../)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_libraries(${HIREDIS_LIBRARY_DEBUG})
else()
  link_libraries(${HIREDIS_LIBRARY_RELEASE})
endif()

set(REDIS_PLUS_PLUS_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/redis-plus-plus/install/include")
set(REDIS_PLUS_PLUS_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/redis-plus-plus/install/lib/redis++_static.lib")
include_directories(${REDIS_PLUS_PLUS_HEADER})
link_libraries(${REDIS_PLUS_PLUS_LIB})


add_executable(${project_name}
  ${header_files}
  ${source_files}
)
