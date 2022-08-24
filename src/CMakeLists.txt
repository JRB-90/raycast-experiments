cmake_minimum_required(VERSION 3.8)

find_package(SDL2 REQUIRED)

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

target_include_directories(CRayCaster PUBLIC
	"../lib"
	$ENV{SDL2_INCLUDE_DIRS}
)

target_link_directories(CRayCaster PUBLIC
	"../lib"
	$ENV{SDL2_LIBRARIES}
)

target_link_libraries(CRayCaster PRIVATE
	SDL2
	SDL2main
	dlist
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CRayCaster PROPERTY C_STANDARD 11)
endif()
