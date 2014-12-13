
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(zlib)

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
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY(pg_zlib STATIC

	adler32.c
	compress.c
	crc32.c
	crc32.h
	deflate.c
	deflate.h
	gzio.c
	infback.c
	inffast.c
	inffast.h
	inffixed.h
	inflate.c
	inflate.h
	inftrees.c
	inftrees.h
	trees.c
	trees.h
	uncompr.c
	zconf.h
	zlib.h
	zutil.c
	zutil.h

	contrib/minizip/crypt.h
	contrib/minizip/ioapi.c
	contrib/minizip/ioapi.h
	contrib/minizip/iostring.cpp
	contrib/minizip/iostring.h
	contrib/minizip/unzip.c
	contrib/minizip/unzip.h
	contrib/minizip/zip.c
	contrib/minizip/zip.h

)

