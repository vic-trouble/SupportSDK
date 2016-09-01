ExternalProject_Add(gtest 
	GIT_REPOSITORY https://github.com/google/googletest GIT_TAG "release-1.8.0"
	UPDATE_COMMAND ""
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/gtest)
