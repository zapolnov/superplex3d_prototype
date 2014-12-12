
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(packer)

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/defines.cmake)

INCLUDE_DIRECTORIES(
	${common_SOURCE_DIR}
	${zlib_SOURCE_DIR}
)

ADD_EXECUTABLE(packer

	packer.cpp

)

TARGET_LINK_LIBRARIES(packer pg_zlib)
