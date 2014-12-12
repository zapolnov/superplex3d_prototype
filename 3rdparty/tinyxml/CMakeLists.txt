
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(tinyxml)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY(tinyxml STATIC
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	tinyxml.h
)
