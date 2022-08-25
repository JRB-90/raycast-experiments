cmake_minimum_required(VERSION 3.8)

project(craycaster)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_DIR $ENV{SDL2_DIR})
set(CRAY_DIR "${CMAKE_SOURCE_DIR}/src/cray")
if (UNIX)
	set(MATH_LIBS m dl)
endif(UNIX)

add_subdirectory("src/cray")
add_subdirectory("src/raycastdemo")