
SET(MODULE_PATH "${ROOT_PROJECT_SOURCE_DIR}/3rd/spdlog")

include_directories("${MODULE_PATH}/include")
add_definitions(-DSPDLOG_USE_STD_FORMAT)