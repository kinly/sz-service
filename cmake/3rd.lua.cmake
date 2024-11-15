SET(MODULE_PATH "${ROOT_PROJECT_SOURCE_DIR}/3rd/lua")

SET(MODULE_NAME "lua")

project(lua)

FILE(GLOB lua_sources ${MODULE_PATH}/*.c)
list(REMOVE_ITEM lua_sources
  "${MODULE_PATH}/lua.c"
  "${MODULE_PATH}/luac.c"
  "${MODULE_PATH}/onelua.c"
)

add_library(${MODULE_NAME} STATIC ${lua_sources})

target_include_directories(${MODULE_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${MODULE_PATH}>
)

set_target_properties(${MODULE_NAME} PROPERTIES
  OUTPUT_DIRECTORY ${MODULE_PATH}/lib
  ARCHIVE_OUTPUT_DIRECTORY ${MODULE_PATH}/lib
)

SET(LUA_HEADER "${MODULE_PATH}")

if (MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(LUA_LIBRARIES_NAME "${MODULE_NAME}d.lib")
  else()
    SET(LUA_LIBRARIES_NAME "${MODULE_NAME}.lib")
  endif()
else()
  SET(LUA_LIBRARIES_NAME "lib${MODULE_NAME}.a")
endif()

SET(LUA_LIBRARIES "${MODULE_PATH}/lib/${CMAKE_BUILD_TYPE}/${LUA_LIBRARIES_NAME}")

INCLUDE_DIRECTORIES("${LUA_HEADER}")
LINK_LIBRARIES("${LUA_LIBRARIES}")
