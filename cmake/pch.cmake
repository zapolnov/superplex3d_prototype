
IF(MSVC)

	MACRO(SETUP_PRECOMPILED_HEADER target pch_source pch_header)

		SET(INT_DIR "${CMAKE_CURRENT_BINARY_DIR}/./${CMAKE_CFG_INTDIR}")

		SET(pch_binary "${INT_DIR}/${target}.pch")
		GET_FILENAME_COMPONENT(pch_filename "${pch_header}" NAME)

		GET_TARGET_PROPERTY(source_files ${target} SOURCES)
		SET_SOURCE_FILES_PROPERTIES(${source_files}
			PROPERTIES COMPILE_FLAGS "/Yu${pch_filename} /Fp${pch_binary} /FI${pch_filename}")

		SET_SOURCE_FILES_PROPERTIES(${pch_source}
			PROPERTIES COMPILE_FLAGS "/Yc${pch_filename} /Fp${pch_binary}")

	ENDMACRO(SETUP_PRECOMPILED_HEADER)

ELSE(MSVC)

	MACRO(SETUP_PRECOMPILED_HEADER target pch_source pch_header)
	ENDMACRO(SETUP_PRECOMPILED_HEADER)

ENDIF(MSVC)
