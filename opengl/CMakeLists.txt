
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_opengl)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

ADD_LIBRARY(pg_opengl STATIC

	context.h
	display_list.h
	glext.h
	glxext.h
	texture.cpp
	texture.h
	wglext.h

)

TARGET_LINK_LIBRARIES(pg_opengl ${OPENGL_gl_LIBRARY})
