include(ExternalProject)

add_subdirectory(assimp)
add_subdirectory(freetype)
add_subdirectory(glew)
add_subdirectory(SOIL)
add_subdirectory(tinyxml2)

if(WITH_TESTS)
	add_subdirectory(gtest)
endif()
