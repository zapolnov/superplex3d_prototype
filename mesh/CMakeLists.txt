
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_mesh)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_BINARY_DIR}
)

QT4_WRAP_CPP(MOC_FILES mesh_material.h)

SET(pg_mesh_SRC
	formats/wavefront_obj/obj_loader.cpp
	formats/wavefront_obj/obj_loader.h
	formats/wavefront_obj/obj_plugin.cpp
	formats/wavefront_obj/obj_plugin.h
	formats/wavefront_obj/obj_reader.cpp
	formats/wavefront_obj/obj_reader.h
	helpers/mesh_format_reader_helper.cpp
	helpers/mesh_format_reader_helper.h
	interfaces/mesh_format_plugin.h
	interfaces/mesh_format_reader.h
	interfaces/mesh_format_writer.h
	index_buffer.cpp
	index_buffer.h
	mesh_file.cpp
	mesh_file.h
	mesh_material.cpp
	mesh_material.h
	vertex_buffer.cpp
	vertex_buffer.h
	vertex_format.cpp
	vertex_format.h
	${MOC_FILES}
)

ADD_LIBRARY(pg_mesh SHARED EXCLUDE_FROM_ALL ${pg_mesh_SRC})
ADD_LIBRARY(pg_mesh_static STATIC EXCLUDE_FROM_ALL ${pg_mesh_SRC})

SETUP_LIBRARY(pg_mesh)
TARGET_LINK_LIBRARIES(pg_mesh pg_core pg_tbb)
SET_PROPERTY(TARGET pg_mesh PROPERTY COMPILE_DEFINITIONS "MESH_DLL" APPEND)

TARGET_LINK_LIBRARIES(pg_mesh_static pg_core_static pg_tbb)
SET_PROPERTY(TARGET pg_mesh_static PROPERTY COMPILE_DEFINITIONS "MESH_STATIC" APPEND)
SET_PROPERTY(TARGET pg_mesh_static PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
