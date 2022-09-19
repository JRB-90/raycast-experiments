cmake_minimum_required(VERSION 3.8)

SET(SDL2_DIR $ENV{SDL2_DIR})

find_package(SDL2 REQUIRED)

add_library(crsdl2
	crsdl2_display.h
	crsdl2_display.c
	crsdl2_input.h
	crsdl2_input.c
)

include_directories(crsdl2 PUBLIC
	${SDL2_INCLUDE_DIRS}
	${CRAY_DIR}
)

target_link_libraries(crsdl2 PRIVATE
	${SDL2_LIBRARIES}
	cray
)
