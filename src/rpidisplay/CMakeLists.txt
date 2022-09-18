cmake_minimum_required(VERSION 3.8)

include_directories(rpidisplay PUBLIC
)

add_executable(rpidisplay
	main.c
    rpifb.h
	rpifb.c
)

target_link_directories(rpidisplay PUBLIC
)

target_link_libraries(rpidisplay PUBLIC
	${MATH_LIBS}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rpidisplay PROPERTY C_STANDARD 11)
endif()
