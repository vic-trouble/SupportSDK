set ("${PROJECT}_BINARY_DIR" Binaries/CMake)
set ("${PROJECT}_SOURCE_DIR" Math:Utilities:GameCore)
set ("${PROJECT}_LIB_DIR" lib)

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
set (CMAKE_LIBRARY_PATH ${${PROJECT}_LIB_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_BUILD_TYPE Debug)

set (NOT_DEFIED_COMPILER "Compiler is not defined.")

if ( WITH_DEBUG_MODE )
     ADD_DEFINITIONS( -D_DEBUG=1)
endif()

if ( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(MY_CXX_FLAGS  "-Wall -std=c++11 -std=gnu++11 -fmessage-length=0 -v")
    set(CMAKE_CXX_FLAGS "-O0 ${MY_CXX_FLAGS}")
    # я отключил настройку разных флагов для релиза и отладки. Пока что не нужно.
    #set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-reorder-blocks -fno-schedule-insns -fno-inline")
    #set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
else ()
    #message (FATAL_ERROR ${NOT_DEFIED_COMPILER})
endif ()