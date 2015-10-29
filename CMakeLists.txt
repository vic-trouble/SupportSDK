cmake_minimum_required (VERSION 2.6)

# Setup OgreMain project
add_subdirectory(SupportSDK)

# The version number.
set (SDK_VERSION_MAJOR 1)
set (SDK_VERSION_MINOR 0)

cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0003 OLD)

OPTION(WITH_DEBUG_MODE "Build with debug mode" ON)

message ("Starting cmake")

# path settings
include (SupportSDK/SupportSDK.cmake)

# в текущем проекте ничего не нужно дополнительно компилировать
set (LIBRARIES)
# следующий код нужен для компиляции и подключения сторонних библиотек 
    foreach (LIBRARY ${LIBRARIES})
        find_library("${LIBRARY}_FOUND" ${LIBRARY})
        message(STATUS "Check the ${LIBRARY} is installed: " ${${LIBRARY}_FOUND})
        if ( "${${LIBRARY}_FOUND}" STREQUAL "${LIBRARY}_FOUND-NOTFOUND" )
            message(STATUS "Adding library sources")
            add_subdirectory (../${LIBRARY} lib/${LIBRARY})
        endif ()
    endforeach ()


# no additional targets
set (TARGETS "")

set (HEADERS "")

message ( STATUS "SOURCES: ${SOURCES}")