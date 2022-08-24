cmake_minimum_required(VERSION 3.8)

project(dlist)

add_library(dlist STATIC
    dlist.h
    dlist.c
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dlist PROPERTY C_STANDARD 11)
endif()
