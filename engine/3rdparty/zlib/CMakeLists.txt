
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(zlib)

DISABLE_WARNINGS()

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
