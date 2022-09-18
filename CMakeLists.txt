cmake_minimum_required(VERSION 3.8)

project(craycaster)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_DIR $ENV{SDL2_DIR})
set(CRAY_DIR "${CMAKE_SOURCE_DIR}/src/cray")

if (UNIX)
	set(MATH_LIBS m dl)
endif(UNIX)

if (MSVC)
    add_compile_options(/W3)
    #add_compile_options(/WX)
else()
   #add_compile_options(-Wall -Wextra -pedantic -Werror)
   #add_compile_options(-Wall) 
   #add_compile_options(-pg)
   #add_compile_options(-g)
   #add_compile_options(-o6)
endif()

add_subdirectory("src/cray")
add_subdirectory("src/raycastdemo")
add_subdirectory("src/crprofile")

if (UNIX)
    add_subdirectory("src/rpitest")
    add_subdirectory("src/rpidisplay")
endif(UNIX)
