SET(MODULE_PATH "${ROOT_PROJECT_SOURCE_DIR}/3rd/hiredis/install")

# options
SET(BUILD_SHARED_LIBS OFF)
SET(ENABLE_SSL OFF)
SET(DISABLE_TESTS OFF)
SET(ENABLE_SSL_TESTS OFF)
SET(ENABLE_EXAMPLES OFF)
SET(ENABLE_ASYNC_TESTS OFF)

SET(HIREDIS_HEADER "${MODULE_PATH}/include")

message("${CMAKE_BUILD_TYPE} build_type")

if (MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("===============1================= ${CMAKE_BUILD_TYPE}")
    SET(HIREDIS_LIBRARIES "${MODULE_PATH}/lib/hiredisd.lib")
  else()
    message("===============2================= ${CMAKE_BUILD_TYPE}")
    SET(HIREDIS_LIBRARIES "${MODULE_PATH}/lib/hiredis.lib")
  endif()
else()
  SET(HIREDIS_LIBRARIES "${MODULE_PATH}/lib/libhiredis.a")
endif()

INCLUDE_DIRECTORIES("${HIREDIS_HEADER}")
LINK_LIBRARIES("${HIREDIS_LIBRARIES}")
