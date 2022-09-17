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

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rpitest PROPERTY C_STANDARD 11)
endif()
