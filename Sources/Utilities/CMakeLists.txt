# Math

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories(
  BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/
)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(Utilities SHARED ${HEADER_FILES} ${SOURCE_FILES})