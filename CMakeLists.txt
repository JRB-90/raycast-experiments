cmake_minimum_required(VERSION 3.8)

project(craycaster)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_DIR $ENV{SDL2_DIR})
set(CRAY_DIR "${CMAKE_SOURCE_DIR}/src/cray")
if (UNIX)
	set(MATH_LIBS m dl)
endif(UNIX)

if (MSVC)
    #add_compile_options(/W3 /WX)
else()
    #add_compile_options(-Wall -Wextra -pedantic -Werror)
    #add_compile_options(-pg)
endif()

add_subdirectory("src/cray")
add_subdirectory("src/raycastdemo")