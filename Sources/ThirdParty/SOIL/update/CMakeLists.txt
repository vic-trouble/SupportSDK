cmake_minimum_required(VERSION 3.0)

project(SOIL)

add_library(SOIL STATIC
    src/image_DXT.c
    src/image_helper.c
    src/SOIL.c
    src/stb_image_aug.c
    )

install(TARGETS SOIL ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES src/SOIL.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
