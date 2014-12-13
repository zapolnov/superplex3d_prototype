/*
 * Mesa 3-D graphics library
 * Version:  7.5
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
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
 * \file imports.h
 * Standard C library function wrappers.
 *
 * This file provides wrappers for all the standard C library functions
 * like malloc(), free(), printf(), getenv(), etc.
 */


#ifndef IMPORTS_H
#define IMPORTS_H


#include "compiler.h"
#include "glheader.h"


#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************************/
/** Memory macros */
/*@{*/

#define MALLOC_STRUCT(T)   (struct T *) _mesa_malloc(sizeof(struct T))
/** Allocate and zero a structure of type \p T */
#define CALLOC_STRUCT(T)   (struct T *) _mesa_calloc(sizeof(struct T))

/** Copy \p BYTES bytes from \p SRC into \p DST */
#define MEMCPY( DST, SRC, BYTES)   _mesa_memcpy(DST, SRC, BYTES)



/**********************************************************************
 * Functions
 */

extern void *
_mesa_malloc( size_t bytes );

extern void *
_mesa_calloc( size_t bytes );

extern void
_mesa_free( void *ptr );

extern void *
_mesa_align_malloc( size_t bytes, unsigned long alignment );

extern void
_mesa_align_free( void *ptr );

extern void *
_mesa_align_realloc(void *oldBuffer, size_t oldSize, size_t newSize,
                    unsigned long alignment);

extern void *
_mesa_realloc( void *oldBuffer, size_t oldSize, size_t newSize );

extern void *
_mesa_memcpy( void *dest, const void *src, size_t n );

extern void
_mesa_memset( void *dst, int val, size_t n );

extern void
_mesa_bzero( void *dst, size_t n );

extern char *
_mesa_strcpy( char *dest, const char *src );

extern char *
_mesa_strncpy( char *dest, const char *src, size_t n );

extern size_t
_mesa_strlen( const char *s );

extern int
_mesa_strcmp( const char *s1, const char *s2 );

extern int
_mesa_strncmp( const char *s1, const char *s2, size_t n );

extern char *
_mesa_strdup( const char *s );

extern int
_mesa_atoi( const char *s );

extern double
_mesa_strtod( const char *s, char **end );

extern int
_mesa_sprintf( char *str, const char *fmt, ... );

extern int
_mesa_snprintf( char *str, size_t size, const char *fmt, ... );

extern void
_mesa_fprintf( FILE *f, const char *fmtString, ... );

extern int 
_mesa_vsprintf( char *str, const char *fmt, va_list args );

extern void
_mesa_warning( __GLcontext *gc, const char *fmtString, ... );

extern void
_mesa_problem( const __GLcontext *ctx, const char *fmtString, ... );

extern void
_mesa_error( __GLcontext *ctx, GLenum error, const char *fmtString, ... );

#ifdef __cplusplus
}
#endif


#endif /* IMPORTS_H */
