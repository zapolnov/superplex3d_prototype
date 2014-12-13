
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_math)

ADD_LIBRARY(pg_math STATIC
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
)

TARGET_LINK_LIBRARIES(pg_math pg_core)
