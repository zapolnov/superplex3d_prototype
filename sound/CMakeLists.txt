
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_sound)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${openal_soft_SOURCE_DIR}/include
	${libogg_SOURCE_DIR}/include
	${libvorbis_SOURCE_DIR}/include
)

QT4_WRAP_CPP(MOC_FILES
	ogg_stream.h
	openal_context.h
	thread.h
)

SET(pg_sound_SRC
	functions.cpp
	functions.h
	ogg_stream.cpp
	ogg_stream.h
	openal_context.cpp
	openal_context.h
	riff.cpp
	riff.h
	sound_file.cpp
	sound_file.h
	thread.cpp
	thread.h
	${MOC_FILES}
)

ADD_LIBRARY(pg_sound STATIC ${pg_sound_SRC})
TARGET_LINK_LIBRARIES(pg_sound pg_core pg_vorbis pg_ogg)
#ADD_DEPENDENCIES(pg_sound pg_openal)
