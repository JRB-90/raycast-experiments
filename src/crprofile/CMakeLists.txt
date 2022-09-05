cmake_minimum_required(VERSION 3.8)

include_directories(crprofile PUBLIC
	${CRAY_DIR}
)

add_executable(crprofile
	main.c
)

target_link_directories(crprofile PUBLIC
	${CRAY_DIR}
)

target_link_libraries(crprofile PUBLIC
	${MATH_LIBS}
	cray
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET crprofile PROPERTY C_STANDARD 11)
endif()
