
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(lines)

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

TARGET_LINK_LIBRARIES(lines pg_casual)
SET_TARGET_PROPERTIES(lines PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
