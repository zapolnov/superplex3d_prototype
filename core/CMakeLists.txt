
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(pg_core)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)
INCLUDE(${cmake_SOURCE_DIR}/pch.cmake)

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

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_BINARY_DIR}
)

QT4_WRAP_CPP(MOC_FILES
	clock.h
	logger.h
)

SET(pg_core_SRC
	../interfaces/file_system.cpp
	../interfaces/file_system.h
	../interfaces/loadable.h
	atom.h
	atom_table.cpp
	atom_table.h
	bit_array.h
	clock.cpp
	clock.h
	common.cpp
	common.h
	exception.cpp
	exception.h
	ext_io_device.cpp
	ext_io_device.h
	fixed_pool.h
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
	unique_id.cpp
	unique_id.h
	utility.cpp
	utility.h
	variant.cpp
	variant.h
	${MOC_FILES}
)

ADD_LIBRARY(pg_core SHARED EXCLUDE_FROM_ALL ${pg_core_SRC})
ADD_LIBRARY(pg_core_static STATIC EXCLUDE_FROM_ALL ${pg_core_SRC})

SETUP_LIBRARY(pg_core)
SET_PROPERTY(TARGET pg_core PROPERTY COMPILE_DEFINITIONS "CORE_DLL" APPEND)
TARGET_LINK_LIBRARIES(pg_core pg_tbb ${QT_PG_QTCORE_LIBRARY})

SET_PROPERTY(TARGET pg_core_static PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
TARGET_LINK_LIBRARIES(pg_core_static pg_tbb)
