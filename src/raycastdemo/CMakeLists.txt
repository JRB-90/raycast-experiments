cmake_minimum_required(VERSION 3.8)

include_directories(raycastdemo PUBLIC
	${CRAY_DIR}
)

add_executable(raycastdemo
	main.c
    raysettings.h
    raysettings.c
)

target_link_directories(raycastdemo PUBLIC
	${CRAY_DIR}
)

target_link_libraries(raycastdemo PUBLIC
	${MATH_LIBS}
	cray
)

if (CRSDL)
target_link_libraries(raycastdemo PUBLIC
	crsdl2
)
elseif (CRRPI)
target_link_libraries(raycastdemo PUBLIC
	crrpifb
)
endif()
