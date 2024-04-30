set(project_name game-service)
project(${project_name})

include_directories(../3rd/cinatra/include)
include_directories(../3rd/json/include)
include_directories(../3rd/tomlplusplus/include)

file(GLOB header_files *.h *.hpp *.inl)
file(GLOB source_files *.cpp *.c)

source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})

find_package(hiredis)
find_package(redis++)

# # <------------ add hiredis dependency --------------->
# set(HIREDIS_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/hiredis/install/include")
# set(HIREDIS_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/hiredis/install/lib/hiredisd.lib")
# include_directories(${HIREDIS_HEADER})
# link_libraries(${HIREDIS_LIB})
# 
# # <------------ add redis-plus-plus dependency -------------->
# # NOTE: this should be *sw* NOT *redis++*
# set(REDIS_PLUS_PLUS_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/redis-plus-plus/install/include")
# set(REDIS_PLUS_PLUS_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/redis-plus-plus/install/lib/redis++_static.lib")
# include_directories(${REDIS_PLUS_PLUS_HEADER})
# link_libraries(${REDIS_PLUS_PLUS_LIB})


add_executable(${project_name}
  ${header_files}
  ${source_files}
)
