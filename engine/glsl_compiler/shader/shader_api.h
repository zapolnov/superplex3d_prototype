/*
 * Mesa 3-D graphics library
 * Version:  6.5
 *
 * Copyright (C) 2004-2006  Brian Paul   All Rights Reserved.
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


#ifndef SHADER_API_H
#define SHADER_API_H


#include "main/glheader.h"
#include "main/mtypes.h"

extern GLint g_EmitHighLevelInstructions;
extern GLint g_EmitComments;

/**
 * Internal functions
 */

extern void
_mesa_init_shader_state(GLcontext * ctx);

extern void
_mesa_clear_shader_program_data(GLcontext *ctx,
                                struct gl_shader_program *shProg);

extern void
_mesa_free_shader_program_data(GLcontext *ctx,
                               struct gl_shader_program *shProg);

extern void
_mesa_free_shader_program(GLcontext *ctx, struct gl_shader_program *shProg);

extern void
_mesa_reference_shader_program(GLcontext *ctx,
                               struct gl_shader_program **ptr,
                               struct gl_shader_program *shProg);

extern void
_mesa_free_shader(GLcontext *ctx, struct gl_shader *sh);

extern void
_mesa_reference_shader(GLcontext *ctx, struct gl_shader **ptr,
                       struct gl_shader *sh);

extern struct gl_shader *
_mesa_lookup_shader(GLcontext *ctx, GLuint name);


extern void
_mesa_update_shader_textures_used(struct gl_program *prog);

GLuint _mesa_create_shader(GLcontext *ctx, GLenum type);

void _mesa_delete_shader(GLcontext *ctx, GLuint shader);

void _mesa_get_shaderiv(GLcontext *ctx, GLuint name, GLenum pname, GLint *params);

void _mesa_get_shader_info_log(GLcontext *ctx, GLuint shader, GLsizei bufSize,
                          GLsizei *length, GLchar *infoLog);

void _mesa_shader_source(GLcontext *ctx, GLuint shader, const GLchar *source);

void _mesa_compile_shader(GLcontext *ctx, GLuint shaderObj);

#endif /* SHADER_API_H */
