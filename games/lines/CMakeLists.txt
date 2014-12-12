
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(lines)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${common_SOURCE_DIR}
	${opengl_SOURCE_DIR}
	${glfw_SOURCE_DIR}/include
	${pg_casual_SOURCE_DIR}/..
)

GENERATE_PACKED_DATA(PACKED_FILE
	arial13.fnt
	arial25.fnt
	marble_green.jpg
	game_over_eng.dds
	jump.wav
	button.dds
	button_pressed.dds
	sphere.obj
	sphere.mtl
	music.ogg
)

ADD_EXECUTABLE(lines WIN32
	a_star.cpp
	a_star.h
	a_star_state.cpp
	a_star_state.h
	a_star_state_base.cpp
	a_star_state_base.h
	ball.cpp
	ball.h
	lines_game.cpp
	lines_game.h
	${PACKED_FILE}
)

SETUP_TARGET(lines)
TARGET_LINK_LIBRARIES(lines pg_casual)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "ENGINE_STATIC" APPEND)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "MATH_STATIC" APPEND)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "MESH_STATIC" APPEND)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "IMAGE_STATIC" APPEND)
SET_PROPERTY(TARGET lines PROPERTY COMPILE_DEFINITIONS "SOUND_STATIC" APPEND)

SET_TARGET_PROPERTIES(lines PROPERTIES
	OUTPUT_NAME "lines"
	SUFFIX ".exe"
)
