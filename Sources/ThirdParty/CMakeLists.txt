include(ExternalProject)

set(THIRDPARTY_INSTALL_FOLDER ${CMAKE_BINARY_DIR}/ThirdParty/install/${CMAKE_CFG_INTDIR})

add_subdirectory(assimp)
add_subdirectory(freetype)
add_subdirectory(glew)
add_subdirectory(SOIL)
add_subdirectory(tinyxml2)

set_target_properties(assimp freetype glew SOIL tinyxml PROPERTIES FOLDER "ThirdParty")

if(WITH_TESTS)
	add_subdirectory(gtest)
	set_target_properties(gtest PROPERTIES FOLDER "ThirdParty")
endif()
