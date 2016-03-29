#root
cmake_minimum_required (VERSION 2.6)

project (SupportSDK)

set (PROJECT SupportSDK)

# The version number.
set (SDK_VERSION_MAJOR 1)
set (SDK_VERSION_MINOR 0)

cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0003 OLD)

OPTION(WITH_DEBUG_MODE "Build with debug mode" ON)

# path settings
include (SupportSDK.cmake)

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



# specify a precompiled header to use
#use_precompiled_header(OgreMain 
  #"${CMAKE_CURRENT_SOURCE_DIR}/include/OgreStableHeaders.h"
  #"${CMAKE_CURRENT_SOURCE_DIR}/src/OgrePrecompiledHeaders.cpp"
#)


# Setup Math project
add_subdirectory(Math)