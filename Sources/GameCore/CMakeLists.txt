set(ALL_FILES)
file(GLOB ALL_FOLDERS "*")
foreach(FOLDER ${ALL_FOLDERS})
	file(GLOB_RECURSE HEADER_FILES "${FOLDER}/*.h")
	file(GLOB_RECURSE SOURCE_FILES "${FOLDER}/*.cpp")
	list(APPEND ${FOLDER}_FILES ${HEADER_FILES} ${SOURCE_FILES})
	list(APPEND ALL_FILES ${${FOLDER}_FILES}) 
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

set(LIB_SUFFIX $<$<CONFIG:Debug>:d>)
target_link_libraries(GameCore 
	Utilities 
	${SOIL_LIB_DIR}/SOIL.lib 
	${TINYXML_LIB_DIR}/tinyxml2.lib 
	opengl32.lib 
	glu32.lib 
	${GLEW_LIB_DIR}/libglew_shared${LIB_SUFFIX}.lib
	${ASSIMP_LIB_DIR}/assimp-vc140-mt.lib 
	${FREETYPE_LIB_DIR}/freetype${LIB_SUFFIX}.lib)

add_dependencies(GameCore SOIL tinyxml glew assimp freetype)


macro(GroupSources curdir)
	file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
	foreach(child ${children})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			GroupSources(${curdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			string(REPLACE "src" "Common" groupname ${groupname})
			source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
		endif()
	endforeach()
 endmacro()

 # Execute the macro

 GroupSources(GameCore)


 function(_generate_vs_source_groups GROUP_NAME TOP_DIRECTORY CURRENT_SUBDIRECTORY RESULT_FILES)

    file(GLOB headers 
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.h
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.hpp
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.hh
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.hxx
    )

    file(GLOB sources 
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.c
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.cpp
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.cc
         ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*.cxx
    )

    string(REPLACE "/" "\\" subgroups "${CURRENT_SUBDIRECTORY}")

    source_group("${GROUP_NAME}\\${subgroups}" FILES ${headers} ${sources})

    list(APPEND result_files ${headers} ${sources})

    file(GLOB childs RELATIVE ${TOP_DIRECTORY} ${TOP_DIRECTORY}/${CURRENT_SUBDIRECTORY}/*)

    foreach(child ${childs})
        if(IS_DIRECTORY ${TOP_DIRECTORY}/${child})
            _generate_vs_source_groups("${GROUP_NAME}" "${TOP_DIRECTORY}" "${child}" res)
            list(APPEND result_files ${res})
        endif()
    endforeach()

    set(${RESULT_FILES} ${result_files} PARENT_SCOPE)
endfunction()

function(generate_vs_source_groups GROUP_NAME TOP_DIRECTORY RESULT_FILES)
    _generate_vs_source_groups("${GROUP_NAME}" "${TOP_DIRECTORY}" "" ${RESULT_FILES})
    set(${RESULT_FILES} "${${RESULT_FILES}}" PARENT_SCOPE)
endfunction()


# generate_vs_source_groups(GameCore ${PROJECT_SOURCE_DIR}/* ${ALL_FILES})