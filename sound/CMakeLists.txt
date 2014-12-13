
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_sound)

ADD_LIBRARY(pg_sound STATIC
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
)

TARGET_LINK_LIBRARIES(pg_sound pg_core pg_vorbis pg_ogg)

IF(WIN32)
	ADD_DEPENDENCIES(pg_sound pg_openal)
ELSEIF(APPLE)
	FIND_LIBRARY(OpenAL OpenAL)
	TARGET_LINK_LIBRARIES(pg_sound ${OpenAL})
ENDIF()
