
set(SERVICE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

set(BT_DIR "${SERVICE_DIR}/bt")

set(BT_NODE_DIR "${SERVICE_DIR}/bt/bt-node")

file(GLOB SERVICE_HEADER "${SERVICE_DIR}/*.h")
file(GLOB SERVICE_SOURCE "${SERVICE_DIR}/*.cpp")

file(GLOB BT_FILES "${BT_DIR}/*.h" "${BT_DIR}/*.cpp")
file(GLOB BT_NODE_FILES
  "${BT_NODE_DIR}/actions/*.*"
  "${BT_NODE_DIR}/conditions/*.*"
  "${BT_NODE_DIR}/controls/*.*"
  "${BT_NODE_DIR}/decorators/*.*"
)

source_group(TREE
  ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
  ${SERVICE_HEADER}
  ${SERVICE_SOURCE}
  ${BT_FILES}
  ${BT_NODE_FILES}
)

set(ALL_FILES
  ${SERVICE_HEADER}
  ${SERVICE_SOURCE}
  ${BT_FILES}
  ${BT_NODE_FILES}
)
