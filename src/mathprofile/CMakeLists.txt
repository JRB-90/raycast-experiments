cmake_minimum_required(VERSION 3.8)

include_directories(mathprofile PUBLIC
	${CRAY_DIR}
)

add_executable(mathprofile
	main.c
)

target_link_directories(mathprofile PUBLIC
	${CRAY_DIR}
)

target_link_libraries(mathprofile PUBLIC
	${MATH_LIBS}
	cray
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mathprofile PROPERTY C_STANDARD 11)
endif()
