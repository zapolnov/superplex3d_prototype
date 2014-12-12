
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(freetype2)

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

INCLUDE_DIRECTORIES(
	include
	${zlib_SOURCE_DIR}
)

ADD_DEFINITIONS("-DFT2_BUILD_LIBRARY")

ADD_LIBRARY(pg_freetype2 STATIC
	src/autofit/autofit.c
	src/base/ftapi.c
	src/base/ftbase.c
	src/base/ftbbox.c
	src/base/ftbdf.c
	src/base/ftbitmap.c
	src/base/ftcid.c
	src/base/ftdebug.c
	src/base/ftgasp.c
	src/base/ftglyph.c
	src/base/ftgxval.c
	src/base/ftinit.c
	src/base/ftlcdfil.c
	src/base/ftmm.c
	src/base/ftotval.c
	src/base/ftpatent.c
	src/base/ftpfr.c
	src/base/ftstroke.c
	src/base/ftsynth.c
	src/base/ftsystem.c
	src/base/fttype1.c
	src/base/ftwinfnt.c
	src/base/ftxf86.c
	src/bdf/bdf.c
	src/cache/ftcache.c
	src/cff/cff.c
	src/cid/type1cid.c
	src/gxvalid/gxvalid.c
	src/gzip/ftgzip.c
	src/lzw/ftlzw.c
	src/otvalid/otvalid.c
	src/pcf/pcf.c
	src/pfr/pfr.c
	src/psaux/psaux.c
	src/pshinter/pshinter.c
	src/psnames/psnames.c
	src/raster/raster.c
	src/sfnt/sfnt.c
	src/smooth/smooth.c
	src/truetype/truetype.c
	src/type1/type1.c
	src/type42/type42.c
	src/winfonts/winfnt.c
)

TARGET_LINK_LIBRARIES(pg_freetype2 pg_zlib)

IF(APPLE)
	FIND_LIBRARY(Carbon Carbon)
	FIND_LIBRARY(AGL AGL)
	FIND_LIBRARY(AppKit AppKit)
	FIND_LIBRARY(ApplicationServices ApplicationServices)
	FIND_LIBRARY(CoreServices CoreServices)
	TARGET_LINK_LIBRARIES(pg_freetype2 ${Carbon} ${AGL} ${AppKit} ${ApplicationServices} ${CoreServices})
ENDIF()
