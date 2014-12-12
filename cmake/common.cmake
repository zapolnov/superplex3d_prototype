
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${cmake_SOURCE_DIR}/modules")

IF(CMAKE_COMPILER_IS_GNUCC)
 SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall")
 SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Werror")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(CMAKE_COMPILER_IS_GNUCXX)
 SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall")
 SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

INCLUDE(FindThreads)

INCLUDE(FindOpenGL)
INCLUDE(${cmake_SOURCE_DIR}/modules/FindCg.cmake)

FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(
	${OPENGL_INCLUDE_DIR}
	${CG_INCLUDE_PATH}
	${engine_SOURCE_DIR}/include
	${intel_tbb_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
)

##
## Sets up executable target
##
MACRO(SETUP_TARGET target_name)

	SET_TARGET_PROPERTIES(${target_name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		PREFIX ""
	)

ENDMACRO(SETUP_TARGET)

##
## Sets up module target
##
MACRO(SETUP_LIBRARY target_name)

	SET_TARGET_PROPERTIES(${target_name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		SUFFIX ".dll"
		PREFIX ""
	)

ENDMACRO(SETUP_LIBRARY)

##
## Generates packed data
##
MACRO(GENERATE_PACKED_DATA target_name )

	SET(listfile "${CMAKE_CURRENT_BINARY_DIR}/packed_data.lst")
	SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/packed_data.cpp")

	SET(datafiles )
	FILE(REMOVE "${listfile}")
    FOREACH(datafile ${ARGN})
    	LIST(APPEND ${datafiles} "${datafile}")
    	FILE(APPEND "${listfile}" "${datafile}\n")
    ENDFOREACH(datafile)

	ADD_CUSTOM_COMMAND(
		OUTPUT "${outfile}"
		COMMAND packer "${CMAKE_CURRENT_SOURCE_DIR}/data" "${listfile}" "${outfile}"
		MAIN_DEPENDENCY "${listfile}"
		DEPENDS ${datafiles} packer
		COMMENT "Generating packed data file"
	)

	SET(${target_name} ${outfile})

ENDMACRO(GENERATE_PACKED_DATA)


## Dummy macros for Qt support

SET(QT_PG_QTCORE_LIBRARY )

MACRO(QT4_WRAP_CPP outfiles )
	SET(${out_files} "")
ENDMACRO(QT4_WRAP_CPP)
