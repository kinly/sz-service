SET(MODULE_PATH "${CMAKE_SOURCE_DIR}/3rd/hiredis/install")
if(MSVC)
  SET(MODULE_PATH "${MODULE_PATH}/msvc")
else()
  SET(MODULE_PATH "${MODULE_PATH}/linux")
endif()

# SET(CMAKE_DEBUG_POSTFIX d)

FIND_PATH(HIREDIS_INCLUDE_DIR
  NAMES "hiredis.h"
  PATHS "${MODULE_PATH}/include/hiredis"
  NO_DEFAULT_PATH
  DOC "Directory hiredis include"
)

FIND_LIBRARY(HIREDIS_LIBRARY_DEBUG
  NAMES hiredisd
  PATHS "${MODULE_PATH}/lib"
  NO_DEFAULT_PATH
  DOC "Directory hiredis debug library"
)

FIND_LIBRARY(HIREDIS_LIBRARY_RELEASE
  NAMES hiredis
  PATHS "${MODULE_PATH}/lib"
  NO_DEFAULT_PATH
  DOC "Directory hiredis release library"
)

# MARK_AS_ADVANCED(hiredis
#   HIREDIS_INCLUDE_DIR
#   HIREDIS_LIBRARY
# )

IF(HIREDIS_INCLUDE_DIR)
  SET(HIREDIS_FOUND 1 CACHE STRING "Set to 1 if hiredis is found, otherwise 0")
ELSE (HIREDIS_INCLUDE_DIR)
  SET(HIREDIS_FOUND 0 CACHE STRING "Set to 1 if hiredis is found, otherwise 0")
ENDIF(HIREDIS_INCLUDE_DIR)

IF(HIREDIS_FOUND)
  MESSAGE(STATUS "find hiredis library..")
ELSE(HIREDIS_FOUND)
  MESSAGE(FATAL_ERROR "cannot find hiredis library, please check it..")
ENDIF(HIREDIS_FOUND)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(hiredis
  DEFAULT_MSG
  HIREDIS_LIBRARY_DEBUG
  HIREDIS_LIBRARY_RELEASE
  HIREDIS_INCLUDE_DIR
)