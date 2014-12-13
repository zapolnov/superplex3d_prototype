/*
 * Mesa 3-D graphics library
 * Version:  7.3
 *
 * Copyright (C) 2004-2008  Brian Paul   All Rights Reserved.
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


#include "glheader.h"
#include "imports.h"
#include "mtypes.h"
#include "shaders.h"
#include "shader/shader_api.h"


/**
 * These are basically just wrappers/adaptors for calling the
 * ctx->Driver.foobar() GLSL-related functions.
 *
 * Things are biased toward the OpenGL 2.0 functions rather than the
 * ARB extensions (i.e. the ARB functions are layered on the 2.0 functions).
 *
 * The general idea here is to allow enough modularity such that a
 * completely different GLSL implemenation can be plugged in and co-exist
 * with Mesa's native GLSL code.
 */



void
_mesa_CompileShaderARB(GLhandleARB shaderObj)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_compile_shader(ctx, shaderObj);
}


GLuint
_mesa_CreateShader(GLenum type)
{
   GET_CURRENT_CONTEXT(ctx);
   return _mesa_create_shader(ctx, type);
}


void
_mesa_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_get_shaderiv(ctx, shader, pname, params);
}


void
_mesa_GetShaderInfoLog(GLuint shader, GLsizei bufSize,
                       GLsizei *length, GLchar *infoLog)
{
   GET_CURRENT_CONTEXT(ctx);
   _mesa_get_shader_info_log(ctx, shader, bufSize, length, infoLog);
}


/**
 * Called via glShaderSource() and glShaderSourceARB() API functions.
 * Basically, concatenate the source code strings into one long string
 * and pass it to ctx->Driver.ShaderSource().
 */
void
_mesa_ShaderSourceARB(GLhandleARB shaderObj, GLsizei count,
                      const GLcharARB ** string, const GLint * length)
{
   GET_CURRENT_CONTEXT(ctx);
   GLint *offsets;
   GLsizei i, totalLength;
   GLcharARB *source;

   if (!shaderObj || string == NULL) {
      _mesa_error(ctx, GL_INVALID_VALUE, "glShaderSourceARB");
      return;
   }

   /*
    * This array holds offsets of where the appropriate string ends, thus the
    * last element will be set to the total length of the source code.
    */
   offsets = (GLint *) _mesa_malloc(count * sizeof(GLint));
   if (offsets == NULL) {
      _mesa_error(ctx, GL_OUT_OF_MEMORY, "glShaderSourceARB");
      return;
   }

   for (i = 0; i < count; i++) {
      if (string[i] == NULL) {
         _mesa_free((GLvoid *) offsets);
         _mesa_error(ctx, GL_INVALID_OPERATION, "glShaderSourceARB(null string)");
         return;
      }
      if (length == NULL || length[i] < 0)
         offsets[i] = _mesa_strlen(string[i]);
      else
         offsets[i] = length[i];
      /* accumulate string lengths */
      if (i > 0)
         offsets[i] += offsets[i - 1];
   }

   /* Total length of source string is sum off all strings plus two.
    * One extra byte for terminating zero, another extra byte to silence
    * valgrind warnings in the parser/grammer code.
    */
   totalLength = offsets[count - 1] + 2;
   source = (GLcharARB *) _mesa_malloc(totalLength * sizeof(GLcharARB));
   if (source == NULL) {
      _mesa_free((GLvoid *) offsets);
      _mesa_error(ctx, GL_OUT_OF_MEMORY, "glShaderSourceARB");
      return;
   }

   for (i = 0; i < count; i++) {
      GLint start = (i > 0) ? offsets[i - 1] : 0;
      _mesa_memcpy(source + start, string[i],
                   (offsets[i] - start) * sizeof(GLcharARB));
   }
   source[totalLength - 1] = '\0';
   source[totalLength - 2] = '\0';

#if 0
   if (0) {
      GLcharARB *newSource;

      newSource = _mesa_read_shader("newshader.frag");
      if (newSource) {
         _mesa_free(source);
         source = newSource;
      }
   }
#endif

   _mesa_shader_source(ctx, shaderObj, source);

   _mesa_free(offsets);
}
