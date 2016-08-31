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
