cmake_minimum_required(VERSION 3.8)

SET(SDL2_DIR $ENV{SDL2_DIR})

find_package(SDL2 REQUIRED)

include_directories(CRayCaster PUBLIC
	"../lib"
	${SDL2_INCLUDE_DIRS}
)

add_executable(CRayCaster
	main.c
    
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
)

target_link_directories(CRayCaster PUBLIC
	"../lib"
	${SDL2_LIBRARIES}
)

target_link_libraries(CRayCaster PRIVATE
	SDL2
	SDL2main
	dlist
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CRayCaster PROPERTY C_STANDARD 11)
endif()
