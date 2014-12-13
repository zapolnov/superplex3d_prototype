
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_png)

DISABLE_WARNINGS()

ADD_LIBRARY(pg_png STATIC
	png.c
	png.h
	pngconf.h
	pngerror.c
	pnggccrd.c
	pngget.c
	pngmem.c
	pngpread.c
	pngread.c
	pngrio.c
	pngrtran.c
	pngrutil.c
	pngset.c
	pngtrans.c
	pngvcrd.c
	pngwio.c
	pngwrite.c
	pngwtran.c
	pngwutil.c
)

TARGET_LINK_LIBRARIES(pg_png pg_zlib)
