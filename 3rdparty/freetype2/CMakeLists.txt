
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

IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_LIBRARY(pg_freetype2 SHARED
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

SET_TARGET_PROPERTIES(pg_freetype2 PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

TARGET_LINK_LIBRARIES(pg_freetype2 pg_zlib)

IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
 SET_TARGET_PROPERTIES(pg_freetype2 PROPERTIES COMPILE_FLAGS "-fvisibility=default -w")
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
