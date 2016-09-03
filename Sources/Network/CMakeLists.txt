file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(Network SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(Network PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(Network Utilities)