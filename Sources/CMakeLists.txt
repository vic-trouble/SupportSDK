#root
cmake_minimum_required (VERSION 2.6)

project(SupportSDK)

set(PROJECT SupportSDK)
set(USE_FOLDERS ON)

# The version number.
set(SDK_VERSION_MAJOR 1)
set(SDK_VERSION_MINOR 0)

cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0003 OLD)

option(WITH_DEBUG_MODE "Build with debug mode" ON)

add_definitions(-DUNICODE)

# Add third party modules
include(ExternalProject)
set(TINYXML_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/tinyxml)
set(TINYXML_INCLUDE_DIR ${TINYXML_INSTALL_DIR}/include)
set(TINYXML_LIB_DIR ${TINYXML_INSTALL_DIR}/lib)
ExternalProject_Add(tinyxml GIT_REPOSITORY https://github.com/leethomason/tinyxml2 CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${TINYXML_INSTALL_DIR})

set(GLEW_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/glew)
set(GLEW_INCLUDE_DIR ${GLEW_INSTALL_DIR}/include)
set(GLEW_LIB_DIR ${GLEW_INSTALL_DIR}/lib)
ExternalProject_Add(glew GIT_REPOSITORY https://github.com/Perlmint/glew-cmake CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${GLEW_INSTALL_DIR})

set(ASSIMP_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/assimp)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_INSTALL_DIR}/include)
set(ASSIMP_LIB_DIR ${ASSIMP_INSTALL_DIR}/lib)
ExternalProject_Add(assimp GIT_REPOSITORY https://github.com/assimp/assimp CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ASSIMP_INSTALL_DIR} -DASSIMP_BUILD_ASSIMP_TOOLS=OFF)

ExternalProject_Add(gtest GIT_REPOSITORY https://github.com/google/googletest CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/gtest)

set(SOIL_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/SOIL)
set(SOIL_INCLUDE_DIR ${SOIL_INSTALL_DIR}/include)
set(SOIL_LIB_DIR ${SOIL_INSTALL_DIR}/lib)
ExternalProject_Add(SOIL 
	URL http://www.lonesock.net/files/soil.zip 
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/SOIL
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SOIL_INSTALL_DIR} 
	PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/SOIL/CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/SOIL/src/SOIL/CMakeLists.txt)

set(FREETYPE_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/freetype)
set(FREETYPE_INCLUDE_DIR ${FREETYPE_INSTALL_DIR}/include)
set(FREETYPE_LIB_DIR ${FREETYPE_INSTALL_DIR}/lib)
ExternalProject_Add(freetype URL http://download.savannah.gnu.org/releases/freetype/ft265.zip CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_INSTALL_DIR})

# Add components
add_subdirectory(DecisionMaking)
add_subdirectory(ExecutionManagement)
add_subdirectory(GameCore)
add_subdirectory(Graphs)
add_subdirectory(Math)
add_subdirectory(Network)
add_subdirectory(Patterns)
add_subdirectory(Terrain)
add_subdirectory(Utilities)
add_subdirectory(WorldInterfacing)
