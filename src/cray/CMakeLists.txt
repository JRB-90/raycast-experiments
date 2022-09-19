cmake_minimum_required(VERSION 3.8)

add_library(cray
	crlist.h
	crlist.c
    crscene.h
	crscene.c
    crrender.h
	crrender.c
    crmath.h
	crmath.c
	crtime.h
    crtime.c
    crprofile.h
	crprofile.c
    crdraw.h
	crdraw.c
    crtypes.h
    crtypes.c
  )

target_link_libraries(cray PRIVATE
	${MATH_LIBS}
)
