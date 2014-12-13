
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_image)

ADD_LIBRARY(pg_image STATIC
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

TARGET_LINK_LIBRARIES(pg_image pg_core pg_tbb pg_jpeg pg_png pg_zlib)
