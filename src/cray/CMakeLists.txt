cmake_minimum_required(VERSION 3.8)

SET(SDL2_DIR $ENV{SDL2_DIR})

find_package(SDL2 REQUIRED)

include_directories(cray PUBLIC
	${SDL2_INCLUDE_DIRS}
)

add_library(cray
	dlist.h
	dlist.c
    display.h
	display.c
    input.h
	input.c
    scene.h
	scene.c
    render.h
	render.c
    craymath.h
	craymath.c
	time.h
    time.c
)

target_link_libraries(cray PRIVATE
	${SDL2_LIBRARIES}
	${MATH_LIBS}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cray PROPERTY C_STANDARD 11)
endif()
