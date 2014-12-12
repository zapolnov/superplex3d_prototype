
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(libvorbis)

ADD_DEFINITIONS("-D_CRT_SECURE_NO_DEPRECATE=1")
ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS=1")
ADD_DEFINITIONS("-D_SCL_SECURE_NO_DEPRECATE=1")
ADD_DEFINITIONS("-D_CRT_NONSTDC_NO_DEPRECATE=1")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	ADD_DEFINITIONS("-D_DEBUG")
ELSE(CMAKE_BUILD_TYPE STREQUAL "Debug")
	ADD_DEFINITIONS("-D_SECURE_SCL=0")
	ADD_DEFINITIONS("-D_HAS_ITERATOR_DEBUGGING=0")
	ADD_DEFINITIONS("-DNDEBUG")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF(MSVC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4005 /wd4305")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4005 /wd4305")
ENDIF(MSVC)

INCLUDE(CheckIncludeFile)

CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)
CONFIGURE_FILE(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${libogg_SOURCE_DIR}/include
	include
	lib
)

SET(DEF_FILE )

IF(WIN32)
	SET(DEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/vorbis.def")
	FILE(READ win32/vorbis.def VORBIS_DEF)
	FILE(READ win32/vorbisfile.def VORBISFILE_DEF)
	STRING(REPLACE "LIBRARY" "" VORBISFILE_DEF "${VORBISFILE_DEF}")
	STRING(REPLACE "EXPORTS" "" VORBISFILE_DEF "${VORBISFILE_DEF}")
	FILE(WRITE "${DEF_FILE}" "${VORBIS_DEF}\n${VORBISFILE_DEF}")
ENDIF(WIN32)

ADD_LIBRARY(pg_vorbis SHARED
	lib/mdct.c
	lib/smallft.c
	lib/block.c
	lib/envelope.c
	lib/window.c
	lib/lsp.c
	lib/lpc.c
	lib/analysis.c
	lib/synthesis.c
	lib/psy.c
	lib/info.c
	lib/floor1.c
	lib/floor0.c
	lib/res0.c
	lib/mapping0.c
	lib/registry.c
	lib/codebook.c
	lib/sharedbook.c
	lib/lookup.c
	lib/bitrate.c
	lib/vorbisfile.c
	lib/vorbisenc.c
	lib/envelope.h
	lib/lpc.h
	lib/lsp.h
	lib/codebook.h
	lib/misc.h
	lib/psy.h
	lib/masking.h
	lib/os.h
	lib/mdct.h
	lib/smallft.h
	lib/highlevel.h
	lib/registry.h
	lib/scales.h
	lib/window.h
	lib/lookup.h
	lib/lookup_data.h
	lib/codec_internal.h
	lib/backends.h
	lib/bitrate.h
	${DEF_FILE}
)

SET_TARGET_PROPERTIES(pg_vorbis PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

TARGET_LINK_LIBRARIES(pg_vorbis pg_ogg)

IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
 SET_TARGET_PROPERTIES(pg_vorbis PROPERTIES COMPILE_FLAGS "-fvisibility=default -w")
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
