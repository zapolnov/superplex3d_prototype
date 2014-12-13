
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_tinyxml)

ADD_LIBRARY(tinyxml STATIC
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	tinyxml.h
)
