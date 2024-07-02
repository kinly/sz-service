SET(MODULE_PATH "${ROOT_PROJECT_SOURCE_DIR}/3rd/BehaviorTree.CPP/install")

include_directories("${MODULE_PATH}/include")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_libraries("${MODULE_PATH}/lib/behaviortree_cppd.lib")
else()
  link_libraries("${CMAKE_CURRENT_SOURCE_DIR}/../3rd/BehaviorTree.CPP/install/lib/behaviortree_cppd.lib")
endif()