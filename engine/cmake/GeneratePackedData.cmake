
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

	IF(NOT CMAKE_CROSSCOMPILING)
		SET(PACKER packer)
	ENDIF()

	ADD_CUSTOM_COMMAND(
		OUTPUT "${outfile}"
		COMMAND "${PACKER}" "${CMAKE_CURRENT_SOURCE_DIR}/data" "${listfile}" "${outfile}"
		MAIN_DEPENDENCY "${listfile}"
		DEPENDS ${datafiles} "${PACKER}"
		COMMENT "Generating packed data file"
	)

	SET(${target_name} ${outfile})

ENDMACRO(GENERATE_PACKED_DATA)
