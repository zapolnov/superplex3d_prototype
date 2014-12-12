
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ttf2fnt)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${freetype2_SOURCE_DIR}/include
	${common_SOURCE_DIR}
)

ADD_EXECUTABLE(ttf2fnt

	ft/font.cpp
	ft/font.h
	ft/glyph.cpp
	ft/glyph.h
	ft/library.cpp
	ft/library.h
	fntfile.h
	ttf2fnt.cpp

)

TARGET_LINK_LIBRARIES(ttf2fnt pg_freetype2 pg_core)
