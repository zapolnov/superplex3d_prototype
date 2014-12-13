
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_opengl)

ADD_LIBRARY(pg_opengl STATIC
	context.h
	glext.h
	glxext.h
	texture.cpp
	texture.h
	wglext.h
)

TARGET_LINK_LIBRARIES(pg_opengl ${OPENGL_gl_LIBRARY})
