/*
 * Mesa 3-D graphics library
 * Version:  6.5.3
 *
 * Copyright (C) 1999-2007  Brian Paul, Tungsten Graphics, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * \mainpage
 *
 * Stand-alone Shading Language compiler.  
 * Basically, a command-line program which accepts GLSL shaders and emits
 * vertex/fragment programs (GPU instructions).
 *
 * This file is basically just a Mesa device driver but instead of building
 * a shared library we build an executable.
 *
 * We can emit programs in three different formats:
 *  1. ARB-style (GL_ARB_vertex/fragment_program)
 *  2. NV-style (GL_NV_vertex/fragment_program)
 *  3. debug-style (a slightly more sophisticated, internal format)
 *
 * Note that the ARB and NV program languages can't express all the
 * features that might be used by a fragment program (examples being
 * uniform and varying vars).  So, the ARB/NV programs that are
 * emitted aren't always legal programs in those languages.
 */

#include <core/logger.h>
#include <core/heap_manager.h>
#include <tbb/mutex.h>
#include <engine/r_opengl.h>
#include "glsl_compiler.h"

EXTERN_C_BEGIN

#include "main/imports.h"
#include "main/shaders.h"
#include "shader/shader_api.h"
#include "shader/prog_print.h"
#include "glapi/glapi.h"
#include "main/shared.h"
#include "main/mtypes.h"
#include "main/macros.h"

EXTERN_C_END

/* Constants */

const uint g_CompilerHeapSize = 1048576 * 20;


/* Function prototypes */

extern "C" void _pg_log_message(FILE * f, const char * msg);
extern "C" void * _pg_malloc(size_t bytes);
extern "C" void _pg_free(void * ptr);


/* Public variables */

GLint g_EmitHighLevelInstructions = GL_FALSE;				/**< If true, high-level instructions like IF and BGNLOOP may be emitted. */
GLint g_EmitComments = GL_FALSE;							/**< If true, comments may be emitted. */


/* Private variables */

static const bool g_LineNumbers = false;					/**< If true, include line numbers into the output. */
static const gl_prog_print_mode g_Mode = PROG_PRINT_ARB;	/**< Can be one of PROG_PRINT_DEBUG, PROG_PRINT_NV or PROG_PRINT_ARB. */
static const bool g_EmitParamInfo = true;					/**< If true, program parameters info should be also emitted. */

static HeapManager * g_MemoryPool = NULL;					/**< Memory pool for the compiler. */
static std::string g_ShaderCode;							/**< Compiled code of the shader. */
static tbb::mutex g_Mutex;									/**< Mutex for serialization. */


/* Private functions */

//
// Compiles the specified shader
//
static
void loadAndCompileShader(GLuint shader, const std::string & text)
{
	GLint stat, length = text.length();
	const GLchar * src = text.c_str();

	_mesa_ShaderSourceARB(shader, 1, &src, &length);
	_mesa_CompileShaderARB(shader);
	_mesa_GetShaderiv(shader, GL_COMPILE_STATUS, &stat);

	if (!stat)
	{
		GLchar log[1000];
		GLsizei len;

		_mesa_GetShaderInfoLog(shader, 1000, &len, log);
		throw std::runtime_error("Problem compiling shader: " + std::string(log));
	}
}

//
// Dumps source code of the compiled program
//
static
void printShaderInstructions(GLuint shader, FILE * f)
{
	GET_CURRENT_CONTEXT(ctx);

	struct gl_shader * sh = _mesa_lookup_shader(ctx, shader);
	struct gl_program * prog = sh->Program;

	_mesa_fprint_program_header(stdout, prog, g_Mode);

	if (g_EmitParamInfo)
		_mesa_print_program_parameters(ctx, prog);

	_mesa_fprint_program_opt(stdout, prog, g_Mode, g_LineNumbers);
}

//
// Compiles the shader
//
static
std::string compileShader(GLenum type, const std::string & text)
{
	tbb::mutex::scoped_lock lock(g_Mutex);
	GLcontext * ctx = NULL;

	g_MemoryPool = new HeapManager(g_CompilerHeapSize);
	if (unlikely(!g_MemoryPool))
		throw std::runtime_error("Out of memory");

	try
	{
		ctx = (GLcontext *)_mesa_calloc(sizeof(GLcontext));
		if (unlikely(!ctx))
			throw std::runtime_error("Out of memory");

		ctx->Shared = _mesa_alloc_shared_state(ctx);
		if (unlikely(!ctx->Shared))
			throw std::runtime_error("Out of memory");

		_mesa_init_shader_state(ctx);

		ctx->Shader.DefaultPragmas.IgnoreOptimize = GL_FALSE;
		ctx->Shader.DefaultPragmas.IgnoreDebug = GL_FALSE;
		ctx->Shader.DefaultPragmas.Debug = GL_FALSE;
		ctx->Shader.DefaultPragmas.Optimize = GL_TRUE;

		ctx->Const.VertexProgram.MaxTemps = MIN2(GL_MaxVertexTemporaries, MAX_PROGRAM_TEMPS);
		ctx->Const.FragmentProgram.MaxTemps = MIN2(GL_MaxFragmentTemporaries, MAX_PROGRAM_TEMPS);
		ctx->Const.VertexProgram.MaxUniformComponents = 1000; // FIXME
		ctx->Const.FragmentProgram.MaxUniformComponents = 1000; // FIXME

		_glapi_set_context((void *)ctx);

		GLuint shader = _mesa_CreateShader(type);
		loadAndCompileShader(shader, text);

		g_ShaderCode.clear();
		printShaderInstructions(shader, stdout);
	}
	catch (...)
	{
		if (ctx->Shared)
			_mesa_free(ctx->Shared);
		if (ctx)	
			_mesa_free(ctx);

		delete g_MemoryPool;
		g_MemoryPool = NULL;

		throw;
	}

	delete g_MemoryPool;
	g_MemoryPool = NULL;

	return g_ShaderCode;
}


/* Public functions */

//
// Receives output from the compiler
//
void _pg_log_message(FILE * f, const char * msg)
{
	if (f == stdout)
		g_ShaderCode.append(msg);
	else
		logger << LOG_ERROR << "%s" << msg;
}

//
// Allocates block of memory
//
void * _pg_malloc(size_t bytes)
{
	Q_ASSERT(g_MemoryPool);
	void * ptr = g_MemoryPool->alloc(bytes);
	if (unlikely(!ptr))
	{
		logger << LOG_ERROR << "GLSL compiler: out of memory";
		abort();
	}
	return ptr;
}

//
// Frees block of memory
//
void _pg_free(void * ptr)
{
	Q_ASSERT(g_MemoryPool);
	if (ptr)
		g_MemoryPool->free(ptr);
}

//
// Compiles the vertex shader
//
std::string GLSL_CompileVertexShader(const std::string & src)
{
	return compileShader(GL_VERTEX_SHADER, src);
}

//
// Compiles the fragment shader
//
std::string GLSL_CompileFragmentShader(const std::string & src)
{
	return compileShader(GL_FRAGMENT_SHADER, src);
}
