cmake_minimum_required(VERSION 3.8)

add_library(crrpifb
	crrpifb_display.h
	crrpifb_display.c
	crrpifb_input.h
	crrpifb_input.c
)

include_directories(crrpifb PUBLIC
	${CRAY_DIR}
)

target_link_libraries(crrpifb PRIVATE
	cray
)
