set(ALL_FILES)
file(GLOB ALL_FOLDERS "*")
foreach(FOLDER ${ALL_FOLDERS})
	file(GLOB_RECURSE HEADER_FILES "${FOLDER}/*.h")
	file(GLOB_RECURSE SOURCE_FILES "${FOLDER}/*.cpp")
	list(APPEND ${FOLDER}_FILES ${HEADER_FILES} ${SOURCE_FILES})
	list(APPEND ALL_FILES ${${FOLDER}_FILES}) 
	message(STATUS ${ALL_FILES})
endforeach()

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(GameCore SHARED ${HEADER_FILES} ${SOURCE_FILES} ${ALL_FILES})

target_include_directories(GameCore PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}/.. 
	${TINYXML_INCLUDE_DIR} 
	${GLEW_INCLUDE_DIR} 
	${ASSIMP_INCLUDE_DIR} 
	${SOIL_INCLUDE_DIR} 
	${FREETYPE_INCLUDE_DIR}/freetype2)    

target_link_libraries(GameCore 
	Utilities 
	${SOIL_LIB_DIR}/SOIL.lib 
	${TINYXML_LIB_DIR}/tinyxml2.lib 
	opengl32.lib 
	glu32.lib 
	${GLEW_LIB_DIR}/libglew_sharedd.lib 
	${ASSIMP_LIB_DIR}/assimp-vc140-mt.lib 
	${FREETYPE_LIB_DIR}/freetyped.lib)

add_dependencies(GameCore SOIL tinyxml glew assimp freetype)

foreach(FOLDER ${ALL_FOLDERS})
	get_filename_component(FOLDER_NAME ${FOLDER} NAME)
	source_group(${FOLDER_NAME} FILES ${${FOLDER}_FILES})
endforeach()
