
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ttf2fnt)

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/defines.cmake)

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

SETUP_TARGET(ttf2fnt)
TARGET_LINK_LIBRARIES(ttf2fnt freetype2 pg_core)

SET_TARGET_PROPERTIES(ttf2fnt PROPERTIES
	OUTPUT_NAME "ttf2fnt"
	SUFFIX ".exe"
)
