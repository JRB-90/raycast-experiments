cmake_minimum_required (VERSION 3.8)

add_executable(CRayCaster
	main.c
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CRayCaster PROPERTY C_STANDARD 17)
endif()
