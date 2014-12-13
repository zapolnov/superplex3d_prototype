
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_mesh)

ADD_LIBRARY(pg_mesh STATIC
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

TARGET_LINK_LIBRARIES(pg_mesh pg_core pg_tbb)
