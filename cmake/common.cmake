
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${cmake_SOURCE_DIR}/modules")

INCLUDE(FindThreads)

INCLUDE(FindOpenGL)

FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(
	${OPENGL_INCLUDE_DIR}
	${engine_SOURCE_DIR}/include
	${intel_tbb_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

##
## Generates packed data
##
MACRO(GENERATE_PACKED_DATA target_name )

	SET(listfile "${CMAKE_CURRENT_BINARY_DIR}/packed_data.lst")
	SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/packed_data.cpp")

	SET(datafiles )
	IF(NOT EXISTS "${listfile}")
		FILE(REMOVE "${listfile}")
    	FOREACH(datafile ${ARGN})
    		LIST(APPEND ${datafiles} "${datafile}")
    		FILE(APPEND "${listfile}" "${datafile}\n")
    	ENDFOREACH(datafile)
    ENDIF()

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
