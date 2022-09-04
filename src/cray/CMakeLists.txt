cmake_minimum_required(VERSION 3.8)

SET(SDL2_DIR $ENV{SDL2_DIR})

find_package(SDL2 REQUIRED)

include_directories(cray PUBLIC
	${SDL2_INCLUDE_DIRS}
)

add_library(cray
	dlist.h
	dlist.c
    crdisplay.h
	crdisplay.c
    crinput.h
	crinput.c
    crscene.h
	crscene.c
    crrender.h
	crrender.c
    crmath.h
	crmath.c
	crtime.h
    crtime.c
    crprofile.h
	crprofile.c
    crdraw.h
	crdraw.c
)

target_link_libraries(cray PRIVATE
	${SDL2_LIBRARIES}
	${MATH_LIBS}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cray PROPERTY C_STANDARD 11)
endif()
