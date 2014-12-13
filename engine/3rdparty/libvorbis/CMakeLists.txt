
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_vorbis)

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF(MSVC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4005 /wd4305")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4005 /wd4305")
ENDIF(MSVC)

INCLUDE(CheckIncludeFile)

CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)
CONFIGURE_FILE(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h")

INCLUDE_DIRECTORIES(lib "${CMAKE_CURRENT_BINARY_DIR}")

ADD_LIBRARY(pg_vorbis STATIC
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
)

TARGET_LINK_LIBRARIES(pg_vorbis pg_ogg)
