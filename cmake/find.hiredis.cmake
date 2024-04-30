set(MODULE_PATH "${PROJECT_SOURCE_DIR}/3rd/hiredis")
message("hiredis module_path: ${MODULE_PATH}")

find_path(HIREDIS_INCLUDE_DIRS NAMES hiredis.h
    PATHS ${MODULE_PATH}
    NO_DEFAULT_PATH
)

find_library(HIREDIS_LIBRARIES NAMES hiredis
    PATHS ${MODULE_PATH}
    NO_DEFAULT_PATH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HIREDIS DEFAULT_MSG HIREDIS_LIBRARIES HIREDIS_INCLUDE_DIRS)

add_subdirectory("3rd/hiredis")
set(HIREDIS_LIBRARIES hiredis)
