cmake_minimum_required(VERSION 3.8)

SET(SDL2_DIR $ENV{SDL2_DIR})

find_package(SDL2 REQUIRED)

add_library(crrpifb
	crrpifb_display.h
	crrpifb_display.c
	crrpifb_input.h
	crrpifb_input.c
    vcio.h
)

include_directories(crrpifb PUBLIC
	${SDL2_INCLUDE_DIRS}
	${CRAY_DIR}
)

target_link_libraries(crrpifb PRIVATE
	${SDL2_LIBRARIES}
	cray
)
