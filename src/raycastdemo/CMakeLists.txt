cmake_minimum_required(VERSION 3.8)

find_package(SDL2 REQUIRED)

include_directories(raycastdemo PUBLIC
	${SDL2_INCLUDE_DIRS}
	${CRAY_DIR}
)

add_executable(raycastdemo
	main.c
    raysettings.h
)

target_link_directories(raycastdemo PUBLIC
	${CRAY_DIR}
)

target_link_libraries(raycastdemo PUBLIC
	${SDL2_LIBRARIES}
	${MATH_LIBS}
	cray
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET raycastdemo PROPERTY C_STANDARD 11)
endif()
