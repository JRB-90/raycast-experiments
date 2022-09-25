cmake_minimum_required(VERSION 3.8)

include_directories(rpitest PUBLIC
	${CRAY_DIR}
)

add_executable(rpitest
	main.c
)

target_link_directories(rpitest PUBLIC
	${CRAY_DIR}
)

target_link_libraries(rpitest PUBLIC
	${MATH_LIBS}
	cray
)

if (CRSDL)
target_link_libraries(rpitest PUBLIC
	crsdl2
)
elseif (CRRPI)
target_link_libraries(rpitest PUBLIC
	crrpifb
)
endif()
