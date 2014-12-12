
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_math)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)
INCLUDE(${cmake_SOURCE_DIR}/pch.cmake)

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

ADD_LIBRARY(pg_math SHARED EXCLUDE_FROM_ALL ${pg_math_SRC})
ADD_LIBRARY(pg_math_static STATIC EXCLUDE_FROM_ALL ${pg_math_SRC})

SETUP_LIBRARY(pg_math)
TARGET_LINK_LIBRARIES(pg_math pg_core ${QT_PG_QTCORE_LIBRARY})
SET_PROPERTY(TARGET pg_math PROPERTY COMPILE_DEFINITIONS "MATH_DLL" APPEND)

TARGET_LINK_LIBRARIES(pg_math_static pg_core_static ${QT_PG_QTCORE_LIBRARY})
SET_PROPERTY(TARGET pg_math_static PROPERTY COMPILE_DEFINITIONS "MATH_STATIC" APPEND)
SET_PROPERTY(TARGET pg_math_static PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
