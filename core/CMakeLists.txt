
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_core)

INCLUDE(CheckCSourceCompiles)

CHECK_C_SOURCE_COMPILES(
"
	#include <math.h>
	int main()
	{
		long double a = 1.5f, b, c = 0.0f;
		b = modfl(a, &c);
		return 0;
	}
"
	HAVE_MODFL
)

CONFIGURE_FILE(cmake_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/cmake_config.h)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}")

ADD_LIBRARY(pg_core STATIC
	../interfaces/file_system.cpp
	../interfaces/file_system.h
	../interfaces/loadable.h
	clock.cpp
	clock.h
	common.cpp
	common.h
	exception.cpp
	exception.h
	ext_io_device.cpp
	ext_io_device.h
	heap_manager.cpp
	heap_manager.h
	logger.cpp
	logger.h
	memory_file.cpp
	memory_file.h
	pointer.h
	ref_counted.cpp
	ref_counted.h
	signal.cpp
	signal.h
	simple_file.cpp
	simple_file.h
	simple_file_system.cpp
	simple_file_system.h
	stack_trace.cpp
	stack_trace.h
	trolltech.cpp
	trolltech.h
	unicode.cpp
	unicode.h
	utility.cpp
	utility.h
	variant.cpp
	variant.h
)

TARGET_LINK_LIBRARIES(pg_core pg_tbb)
