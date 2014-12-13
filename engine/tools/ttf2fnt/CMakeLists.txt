
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ttf2fnt)

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
