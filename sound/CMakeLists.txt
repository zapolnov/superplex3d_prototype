
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

ADD_LIBRARY(pg_sound SHARED EXCLUDE_FROM_ALL ${pg_sound_SRC})
ADD_LIBRARY(pg_sound_static STATIC EXCLUDE_FROM_ALL ${pg_sound_SRC})

SETUP_LIBRARY(pg_sound)
TARGET_LINK_LIBRARIES(pg_sound pg_core vorbisfile libvorbis libogg)
ADD_DEPENDENCIES(pg_sound pg_openal)
SET_PROPERTY(TARGET pg_sound PROPERTY COMPILE_DEFINITIONS "SOUND_DLL" APPEND)

TARGET_LINK_LIBRARIES(pg_sound_static pg_core_static vorbisfile libvorbis libogg)
ADD_DEPENDENCIES(pg_sound_static pg_openal)
SET_PROPERTY(TARGET pg_sound_static PROPERTY COMPILE_DEFINITIONS "SOUND_STATIC" APPEND)
SET_PROPERTY(TARGET pg_sound_static PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
