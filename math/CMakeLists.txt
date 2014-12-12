
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_math)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

QT4_WRAP_CPP(MOC_FILES camera.h)

SET(pg_math_SRC
	aabb.h
	camera.cpp
	camera.h
	CMakeLists.txt
	matrix.cpp
	matrix.h
	plane.cpp
	plane.h
	quaternion.cpp
	quaternion.h
	ray.h
	sphere.cpp
	sphere.h
	utility.h
	vector2.h
	vector3.h
	vector4.h
	${MOC_FILES}
)

ADD_LIBRARY(pg_math STATIC ${pg_math_SRC})
TARGET_LINK_LIBRARIES(pg_math pg_core ${QT_PG_QTCORE_LIBRARY})
