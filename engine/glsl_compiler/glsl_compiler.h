#ifndef __ENGINE__GLSLCOMPILER__GLSLCOMPILER_H__
#define __ENGINE__GLSLCOMPILER__GLSLCOMPILER_H__

/* Includes */

#include <core/common.h>
#include <string>


/* Functions */

/**
 * Compiles the vertex shader.
 * @param src Vertex shader source.
 */
std::string GLSL_CompileVertexShader(const std::string & src);

/**
 * Compiles the fragment shader.
 * @param src Fragment shader source.
 */
std::string GLSL_CompileFragmentShader(const std::string & src);

#endif // __ENGINE__GLSLCOMPILER__GLSLCOMPILER_H__
