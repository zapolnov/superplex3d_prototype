
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

ADD_SUBDIRECTORY(glsl_compiler)

PROJECT(pg_casual)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)
INCLUDE(${cmake_SOURCE_DIR}/pch.cmake)

INCLUDE_DIRECTORIES(
	${common_SOURCE_DIR}
	${zlib_SOURCE_DIR}
	${tinyxml_SOURCE_DIR}
	${glfw_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

SET(machine undefined)
IF(WIN32)
	SET(machine win32)
ENDIF(WIN32)
IF(UNIX)
	SET(machine unix)
ENDIF(UNIX)

#ADD_DEFINITIONS(-DDONT_USE_VBO_AND_VAR)

ADD_LIBRARY(pg_casual STATIC EXCLUDE_FROM_ALL

	terrain/patch.cpp
	terrain/patch.h
	terrain/patch_base.cpp
	terrain/patch_base.h
	terrain/patch_morph_hw.cpp
	terrain/patch_morph_hw.h
	terrain/patch_morph_sw.cpp
	terrain/patch_morph_sw.h
	terrain/pvs.cpp
	terrain/pvs.h
	terrain/terrain.cpp
	terrain/terrain.h
	casual_game.cpp
	casual_game.h
	filesys.cpp
	filesys.h
	input.cpp
	input.h
	r_buffer.cpp
	r_font.cpp
	r_lightning.cpp
	r_main.cpp
	r_material.cpp
	r_mesh.cpp
	r_opengl.cpp
	r_opengl.h
	r_shader.cpp
	r_terrain.cpp
	r_texture.cpp
	render.h
	stdafx.cpp
	stdafx.h
	sys_${machine}.cpp
	system.cpp
	system.h
	zip_file.cpp
	zip_file.h
	zip_file_system.cpp
	zip_file_system.h
	zip_io_handlers.cpp

)

TARGET_LINK_LIBRARIES(pg_casual glsl_compiler pg_image pg_sound pg_math pg_core pg_tbb)
TARGET_LINK_LIBRARIES(pg_casual pg_opengl pg_mesh glfw tinyxml pg_zlib ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(pg_casual ${OPENGL_glu_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "ENGINE_STATIC" APPEND)
SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "MATH_STATIC" APPEND)
SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "MESH_STATIC" APPEND)
SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "IMAGE_STATIC" APPEND)
SET_PROPERTY(TARGET pg_casual PROPERTY COMPILE_DEFINITIONS "SOUND_STATIC" APPEND)

SETUP_PRECOMPILED_HEADER(pg_casual stdafx.cpp stdafx.h)
