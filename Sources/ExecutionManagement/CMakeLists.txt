file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(ExecutionManagement SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(ExecutionManagement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)