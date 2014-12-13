
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

ADD_SUBDIRECTORY(glsl_compiler)

PROJECT(pg_casual)

SET(machine undefined)
IF(WIN32)
	SET(machine win32)
ELSEIF(UNIX)
	SET(machine unix)
ENDIF()

#ADD_DEFINITIONS(-DDONT_USE_VBO_AND_VAR)

ADD_LIBRARY(pg_casual STATIC
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
	r_texture.cpp
	render.h
	sys_${machine}.cpp
	system.cpp
	system.h
	zip_file.cpp
	zip_file.h
	zip_file_system.cpp
	zip_file_system.h
	zip_io_handlers.cpp
)

TARGET_LINK_LIBRARIES(pg_casual glsl_compiler pg_image pg_sound pg_math pg_core pg_tbb
	pg_opengl pg_mesh glfw tinyxml pg_zlib ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT})
