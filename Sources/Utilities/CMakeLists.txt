file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(Utilities SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(Utilities PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.. ${TINYXML_INCLUDE_DIR})
target_link_libraries(Utilities ${TINYXML_LIB_DIR}/tinyxml2.lib)
add_dependencies(Utilities tinyxml)