set(project_name game-service)
project(${project_name})
SET(CMAKE_DEBUG_POSTFIX d)

include(../cmake/3rd.cinatra.cmake)
# include(../cmake/3rd.hiredis.cmake)

include_directories(./)

# include_directories(../3rd/cinatra/include)
include_directories(../3rd/json/include)

include_directories(../3rd/tomlplusplus/include)

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

# BehaviorTree.CPP
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rd/BehaviorTree.CPP/install/include")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_libraries("${CMAKE_CURRENT_SOURCE_DIR}/../3rd/BehaviorTree.CPP/install/lib/behaviortree_cppd.lib")
else()
  link_libraries("${CMAKE_CURRENT_SOURCE_DIR}/../3rd/BehaviorTree.CPP/install/lib/behaviortree_cppd.lib")
endif()

set(header_files
  ./animal.h
  ./battle.h
  ./calendar.h
  ./cfg_animals.h
  ./non-lock.h
  ./player.h
  ./property.h
  ./room.h
  ./snowflake.h
  ./test_helper.h
)

set(source_files
  ./battle.cpp
  ./main.cpp
  ./player.cpp
)

set(bt_files
  bt/bt-helper.cpp
  bt/bt-helper.h
  bt/enum_register.cpp

  bt/bt-node/actions/clear_last_target.cpp
  bt/bt-node/actions/current_ts.cpp
  bt/bt-node/actions/debug_say.cpp
  bt/bt-node/actions/exchange_mode.cpp
  bt/bt-node/actions/get_attr.cpp
  bt/bt-node/actions/inc_attr.cpp
  bt/bt-node/actions/init.data.cpp
  bt/bt-node/actions/init.data.h
  bt/bt-node/actions/init_owner_data.cpp
  bt/bt-node/actions/last_recovery_ts.cpp
  bt/bt-node/actions/queue_pop_back.cpp
  bt/bt-node/actions/queue_pop_front.cpp
  bt/bt-node/actions/queue_push_back.cpp
  bt/bt-node/actions/queue_push_front.cpp
  bt/bt-node/actions/queue_size.cpp
  bt/bt-node/actions/revise_attr.cpp
  bt/bt-node/actions/set_blackboard.cpp
  bt/bt-node/actions/set_last_recovery_ts.cpp

  bt/bt-node/conditions/compare_attr.cpp
  bt/bt-node/conditions/compare_numeric.cpp
  bt/bt-node/conditions/is_target_attack_able.cpp
  bt/bt-node/conditions/is_target_faraway.cpp
  bt/bt-node/conditions/queue_empty.cpp

  bt/bt-node/controls/switch_enum.cpp

  bt/bt-node/decorators/queue_foreach.cpp
  bt/bt-node/decorators/queue_foreach_copied.cpp
  bt/bt-node/decorators/run_once.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${header_files} ${source_files} ${bt_files})

add_executable(${project_name}
  ${header_files}
  ${source_files}
  ${bt_files}
)
