
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_image)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_BINARY_DIR}
	${libpng_SOURCE_DIR}
	${libjpeg_SOURCE_DIR}
	${zlib_SOURCE_DIR}
)

SET(pg_image_SRC
	formats/dds/dds_plugin.cpp
	formats/dds/dds_plugin.h
	formats/dds/dds_reader.cpp
	formats/dds/dds_reader.h
	formats/jpeg/jpeg_plugin.cpp
	formats/jpeg/jpeg_plugin.h
	formats/jpeg/jpeg_reader.cpp
	formats/jpeg/jpeg_reader.h
	formats/png/png_plugin.cpp
	formats/png/png_plugin.h
	formats/png/png_reader.cpp
	formats/png/png_reader.h
	helpers/image_format_reader_helper.cpp
	helpers/image_format_reader_helper.h
	interfaces/image_format_plugin.h
	interfaces/image_format_reader.h
	interfaces/image_format_writer.h
	image_file.cpp
	image_file.h
)

ADD_LIBRARY(pg_image SHARED EXCLUDE_FROM_ALL ${pg_image_SRC})
ADD_LIBRARY(pg_image_static STATIC EXCLUDE_FROM_ALL ${pg_image_SRC})

SETUP_LIBRARY(pg_image)
SET_PROPERTY(TARGET pg_image PROPERTY COMPILE_DEFINITIONS "IMAGE_DLL" APPEND)
TARGET_LINK_LIBRARIES(pg_image pg_core pg_tbb libjpeg libpng zlib)

TARGET_LINK_LIBRARIES(pg_image_static pg_core_static pg_tbb libjpeg libpng zlib)
SET_PROPERTY(TARGET pg_image_static PROPERTY COMPILE_DEFINITIONS "IMAGE_STATIC" APPEND)
SET_PROPERTY(TARGET pg_image_static PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
