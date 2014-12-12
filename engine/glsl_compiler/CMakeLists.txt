
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(glsl_compiler)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/library)

INCLUDE_DIRECTORIES(
	${common_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/library
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${pg_casual_SOURCE_DIR}/..
	${glfw_SOURCE_DIR}/include
)

ADD_EXECUTABLE(syn_to_c
	shader/slang/library/syn_to_c.c
)

ADD_EXECUTABLE(gc_to_bin
   	shader/slang/library/gc_to_bin.c
	shader/grammar/grammar.h
	shader/grammar/grammar_crt.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_shader_syn.h
)

FUNCTION(SYN_TO_C inFile outFile)
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/library/${outFile}
		COMMAND syn_to_c ${CMAKE_CURRENT_SOURCE_DIR}/shader/slang/library/${inFile} > ${CMAKE_CURRENT_BINARY_DIR}/library/${outFile}
		MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/shader/slang/library/${inFile}
		DEPENDS syn_to_c
		COMMENT "Generating library/${outFile}"
	)
ENDFUNCTION(SYN_TO_C)

FUNCTION(GC_TO_BIN shaderType inFile outFile)
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/library/${outFile}
		COMMAND gc_to_bin ${shaderType} ${CMAKE_CURRENT_SOURCE_DIR}/shader/slang/library/${inFile} ${CMAKE_CURRENT_BINARY_DIR}/library/${outFile}
		MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/shader/slang/library/${inFile}
		DEPENDS gc_to_bin
		COMMENT "Generating library/${outFile}"
	)
ENDFUNCTION(GC_TO_BIN)

# Fix for stupid bug of CMake
ADD_CUSTOM_COMMAND(OUTPUT None COMMAND syn_to_c MAIN_DEPENDENCY syn_to_c)
ADD_CUSTOM_COMMAND(OUTPUT None COMMAND gc_to_bin MAIN_DEPENDENCY gc_to_bin)

SYN_TO_C(slang_shader.syn slang_shader_syn.h)
SYN_TO_C(slang_pp_expression.syn slang_pp_expression_syn.h)
SYN_TO_C(slang_pp_version.syn slang_pp_version_syn.h)
SYN_TO_C(slang_pp_directives.syn slang_pp_directives_syn.h)

GC_TO_BIN(1 slang_builtin_120_common.gc slang_builtin_120_common_gc.h)
GC_TO_BIN(1 slang_builtin_120_fragment.gc slang_builtin_120_fragment_gc.h)
GC_TO_BIN(1 slang_common_builtin.gc slang_common_builtin_gc.h)
GC_TO_BIN(1 slang_core.gc slang_core_gc.h)
GC_TO_BIN(1 slang_120_core.gc slang_120_core_gc.h)
GC_TO_BIN(1 slang_fragment_builtin.gc slang_fragment_builtin_gc.h)
GC_TO_BIN(2 slang_vertex_builtin.gc slang_vertex_builtin_gc.h)

ADD_LIBRARY(glsl_compiler STATIC EXCLUDE_FROM_ALL

	glsl_compiler.cpp
	glsl_compiler.h

	glapi/glapi.c
	glapi/glapi.h

	main/compiler.h
	main/config.h
	main/glheader.h
	main/hash.c
	main/hash.h
	main/imports.c
	main/imports.h
	main/macros.h
	main/mfeatures.h
	main/mtypes.h
	main/shaders.h
	main/shaders.c
	main/shared.h
	main/shared.c

	shader/prog_instruction.c
	shader/prog_instruction.h
	shader/prog_optimize.c
	shader/prog_optimize.h
	shader/prog_parameter.c
	shader/prog_parameter.h
	shader/prog_print.c
	shader/prog_print.h
	shader/prog_statevars.c
	shader/prog_statevars.h
	shader/program.c
	shader/program.h
	shader/programopt.c
	shader/programopt.h
	shader/shader_api.c
	shader/shader_api.h

	shader/grammar/grammar.h
	shader/grammar/grammar_mesa.c
	shader/grammar/grammar_mesa.h

	shader/slang/slang_builtin.c
	shader/slang/slang_builtin.h
	shader/slang/slang_codegen.c
	shader/slang/slang_codegen.h
	shader/slang/slang_compile.c
	shader/slang/slang_compile.h
	shader/slang/slang_compile_function.c
	shader/slang/slang_compile_function.h
	shader/slang/slang_compile_operation.c
	shader/slang/slang_compile_operation.h
	shader/slang/slang_compile_struct.c
	shader/slang/slang_compile_struct.h
	shader/slang/slang_compile_variable.c
	shader/slang/slang_compile_variable.h
	shader/slang/slang_emit.c
	shader/slang/slang_emit.h
	shader/slang/slang_ir.c
	shader/slang/slang_ir.h
	shader/slang/slang_label.c
	shader/slang/slang_label.h
	shader/slang/slang_log.c
	shader/slang/slang_log.h
	shader/slang/slang_mem.c
	shader/slang/slang_mem.h
	shader/slang/slang_preprocess.c
	shader/slang/slang_preprocess.h
	shader/slang/slang_print.c
	shader/slang/slang_print.h
	shader/slang/slang_simplify.c
	shader/slang/slang_simplify.h
	shader/slang/slang_storage.c
	shader/slang/slang_storage.h
	shader/slang/slang_typeinfo.c
	shader/slang/slang_typeinfo.h
	shader/slang/slang_utility.c
	shader/slang/slang_utility.h
	shader/slang/slang_vartable.c
	shader/slang/slang_vartable.h

	${CMAKE_CURRENT_BINARY_DIR}/library/slang_shader_syn.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_core_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_120_core_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_builtin_120_fragment_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_common_builtin_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_fragment_builtin_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_pp_expression_syn.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_pp_version_syn.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_vertex_builtin_gc.h
	${CMAKE_CURRENT_BINARY_DIR}/library/slang_pp_directives_syn.h

)

TARGET_LINK_LIBRARIES(glsl_compiler ${OPENGL_gl_LIBRARY})
SET_PROPERTY(TARGET glsl_compiler PROPERTY COMPILE_DEFINITIONS "CORE_STATIC" APPEND)
