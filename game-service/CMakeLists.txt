set(project_name game-service)
project(${project_name})
SET(CMAKE_DEBUG_POSTFIX d)

include_directories(./)
# util
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../util")

# 3rds
include(../cmake/3rd.cinatra.cmake)
include(../cmake/3rd.json.cmake)
include(../cmake/3rd.toml++.cmake)

include(../cmake/3rd.hiredis.cmake)
include(../cmake/3rd.redis++.cmake)
include(../cmake/3rd.behaviorTree.cpp.cmake)
include(../cmake/3rd.spdlog.cmake)

# files
include(./cmake/files.cmake)

add_executable(${project_name}
  ${ALL_FILES}
)

# 生成头文件
# add_custom_target(generate_header_files
#   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/header_files ${CMAKE_CURRENT_BINARY_DIR}/header_files
#   DEPENDS ${header_files}
# )