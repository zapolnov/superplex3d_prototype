/*
 * Mesa 3-D graphics library
 * Version:  7.5
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 * Copyright (C) 2008  VMware, Inc.  All Rights Reserved.
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
 * \file config.h
 * Tunable configuration parameters.
 */

#ifndef MESA_CONFIG_H_INCLUDED
#define MESA_CONFIG_H_INCLUDED


/**
 * \name OpenGL implementation limits
 */
/*@{*/

/**
 * Max number of texture image units.  Also determines number of texture
 * samplers in shaders.
 */
#define MAX_TEXTURE_IMAGE_UNITS 16


/** For any program target/extension */
/*@{*/
#define MAX_PROGRAM_TEMPS 256
#define MAX_VARYING 16     /**< number of float[4] vectors */
#define MAX_SAMPLERS MAX_TEXTURE_IMAGE_UNITS
#define MAX_PROGRAM_INPUTS 32
#define MAX_PROGRAM_OUTPUTS 32
/*@}*/

/** For GL_ARB_draw_buffers */
/*@{*/
#define MAX_DRAW_BUFFERS 4
/*@}*/

#endif /* MESA_CONFIG_H_INCLUDED */
