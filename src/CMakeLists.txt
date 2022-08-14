cmake_minimum_required (VERSION 3.8)

add_executable(CRayCaster
	main.c
    display.h
	display.c
)

target_include_directories(CRayCaster PUBLIC
	$ENV{SDL2_INCLUDE}
)

target_link_directories(CRayCaster PUBLIC
	$ENV{SDL2_LIB}
)

target_link_libraries(CRayCaster PRIVATE
	SDL2
	SDL2main
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CRayCaster PROPERTY C_STANDARD 17)
endif()
