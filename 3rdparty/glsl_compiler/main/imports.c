/**
 * \file imports.c
 * Standard C library function wrappers.
 * 
 * Imports are services which the device driver or window system or
 * operating system provides to the core renderer.  The core renderer (Mesa)
 * will call these functions in order to do memory allocation, simple I/O,
 * etc.
 *
 * Some drivers will want to override/replace this file with something
 * specialized, but that'll be rare.
 *
 * Eventually, I want to move roll the glheader.h file into this.
 *
 * \todo Functions still needed:
 * - scanf
 * - qsort
 * - rand and RAND_MAX
 */

/*
 * Mesa 3-D graphics library
 * Version:  7.1
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
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



#include "imports.h"
#include "mtypes.h"


#define MAXSTRING 4000  /* for vsnprintf() */

#ifdef WIN32
#define vsnprintf _vsnprintf
#elif defined(__IBMC__) || defined(__IBMCPP__) || ( defined(__VMS) && __CRTL_VER < 70312000 )
extern int vsnprintf(char *str, size_t count, const char *fmt, va_list arg);
#ifdef __VMS
#include "vsnprintf.c"
#endif
#endif

extern void _pg_log_message(FILE * f, const char * msg);
extern void * _pg_malloc(size_t bytes);
extern void * _pg_calloc(size_t n, size_t bytes);
extern void _pg_free(void * ptr);

/**********************************************************************/
/** \name Memory */
/*@{*/

/** Wrapper around malloc() */
void *
_mesa_malloc(size_t bytes)
{
   return _pg_malloc(bytes);
}

/** Wrapper around calloc() */
void *
_mesa_calloc(size_t bytes)
{
   void * ptr = _mesa_malloc(bytes);
   if (ptr)
      memset(ptr, 0, bytes);
   return ptr;	
}

/** Wrapper around free() */
void
_mesa_free(void *ptr)
{
   _pg_free(ptr);
}

/**
 * Allocate aligned memory.
 *
 * \param bytes number of bytes to allocate.
 * \param alignment alignment (must be greater than zero).
 * 
 * Allocates extra memory to accommodate rounding up the address for
 * alignment and to record the real malloc address.
 *
 * \sa _mesa_align_free().
 */
void *
_mesa_align_malloc(size_t bytes, unsigned long alignment)
{
   uintptr_t ptr, buf;

   ASSERT( alignment > 0 );

   ptr = (uintptr_t) _mesa_malloc(bytes + alignment + sizeof(void *));
   if (!ptr)
      return NULL;

   buf = (ptr + alignment + sizeof(void *)) & ~(uintptr_t)(alignment - 1);
   *(uintptr_t *)(buf - sizeof(void *)) = ptr;

#ifdef _DEBUG
   /* mark the non-aligned area */
   while ( ptr < buf - sizeof(void *) ) {
      *(unsigned long *)ptr = 0xcdcdcdcd;
      ptr += sizeof(unsigned long);
   }
#endif

   return (void *) buf;
}

/**
 * Free memory which was allocated with either _mesa_align_malloc()
 * or _mesa_align_calloc().
 * \param ptr pointer to the memory to be freed.
 * The actual address to free is stored in the word immediately before the
 * address the client sees.
 */
void
_mesa_align_free(void *ptr)
{
   void **cubbyHole = (void **) ((char *) ptr - sizeof(void *));
   void *realAddr = *cubbyHole;
   _mesa_free(realAddr);
}

/**
 * Reallocate memory, with alignment.
 */
void *
_mesa_align_realloc(void *oldBuffer, size_t oldSize, size_t newSize,
                    unsigned long alignment)
{
   const size_t copySize = (oldSize < newSize) ? oldSize : newSize;
   void *newBuf = _mesa_align_malloc(newSize, alignment);
   if (newBuf && oldBuffer && copySize > 0) {
      _mesa_memcpy(newBuf, oldBuffer, copySize);
   }
   if (oldBuffer)
      _mesa_align_free(oldBuffer);
   return newBuf;
}

/** Reallocate memory */
void *
_mesa_realloc(void *oldBuffer, size_t oldSize, size_t newSize)
{
   const size_t copySize = (oldSize < newSize) ? oldSize : newSize;
   void *newBuffer = _mesa_malloc(newSize);
   if (newBuffer && oldBuffer && copySize > 0)
      _mesa_memcpy(newBuffer, oldBuffer, copySize);
   if (oldBuffer)
      _mesa_free(oldBuffer);
   return newBuffer;
}

/** memcpy wrapper */
void *
_mesa_memcpy(void *dest, const void *src, size_t n)
{
#if defined(SUNOS4)
   return memcpy((char *) dest, (char *) src, (int) n);
#else
   return memcpy(dest, src, n);
#endif
}

/** Wrapper around memset() */
void
_mesa_memset( void *dst, int val, size_t n )
{
#if defined(SUNOS4)
   memset( (char *) dst, (int) val, (int) n );
#else
   memset(dst, val, n);
#endif
}

/** Wrapper around either memset() or bzero() */
void
_mesa_bzero( void *dst, size_t n )
{
#if defined(__FreeBSD__)
   bzero( dst, n );
#else
   memset( dst, 0, n );
#endif
}

/*@}*/


/**********************************************************************/
/** \name String */
/*@{*/

/** Wrapper around strcpy() */
char *
_mesa_strcpy( char *dest, const char *src )
{
   return strcpy(dest, src);
}

/** Wrapper around strncpy() */
char *
_mesa_strncpy( char *dest, const char *src, size_t n )
{
   return strncpy(dest, src, n);
}

/** Wrapper around strlen() */
size_t
_mesa_strlen( const char *s )
{
   return strlen(s);
}

/** Wrapper around strcmp() */
int
_mesa_strcmp( const char *s1, const char *s2 )
{
   return strcmp(s1, s2);
}

/** Wrapper around strncmp() */
int
_mesa_strncmp( const char *s1, const char *s2, size_t n )
{
   return strncmp(s1, s2, n);
}

/**
 * Implemented using _mesa_malloc() and _mesa_strcpy.
 * Note that NULL is handled accordingly.
 */
char *
_mesa_strdup( const char *s )
{
   if (s) {
      size_t l = _mesa_strlen(s);
      char *s2 = (char *) _mesa_malloc(l + 1);
      if (s2)
         _mesa_strcpy(s2, s);
      return s2;
   }
   else {
      return NULL;
   }
}

/** Wrapper around atoi() */
int
_mesa_atoi(const char *s)
{
   return atoi(s);
}

/** Wrapper around strtod() */
double
_mesa_strtod( const char *s, char **end )
{
   return strtod(s, end);
}

/*@}*/


/**********************************************************************/
/** \name I/O */
/*@{*/

/** Wrapper around vsprintf() */
int
_mesa_sprintf( char *str, const char *fmt, ... )
{
   int r;
   va_list args;
   va_start( args, fmt );  
   r = vsprintf( str, fmt, args );
   va_end( args );
   return r;
}

/** Wrapper around vsnprintf() */
int
_mesa_snprintf( char *str, size_t size, const char *fmt, ... )
{
   int r;
   va_list args;
   va_start( args, fmt );  
   r = vsnprintf( str, size, fmt, args );
   va_end( args );
   return r;
}

/** Wrapper around fprintf(), using vsprintf() for the formatting. */
void
_mesa_fprintf( FILE *f, const char *fmtString, ... )
{
   char s[MAXSTRING];
   va_list args;
   va_start( args, fmtString );  
   vsnprintf(s, MAXSTRING, fmtString, args);
   va_end( args );
   _pg_log_message(f, s);
}


/** Wrapper around vsprintf() */
int
_mesa_vsprintf( char *str, const char *fmt, va_list args )
{
   return vsprintf( str, fmt, args );
}

/*@}*/


/**********************************************************************/
/** \name Diagnostics */
/*@{*/

static void
output_if_debug(const char *prefixString, const char *outputString,
                GLboolean newline)
{
#ifdef _DEBUG
      _mesa_fprintf(stderr, "%s: %s", prefixString, outputString);
      if (newline)
         _mesa_fprintf(stderr, "\n");
#endif
}

/**
 * Report a warning (a recoverable error condition) to stderr if
 * either DEBUG is defined or the MESA_DEBUG env var is set.
 *
 * \param ctx GL context.
 * \param fmtString printf()-like format string.
 */
void
_mesa_warning( GLcontext *ctx, const char *fmtString, ... )
{
   char str[MAXSTRING];
   va_list args;
   va_start( args, fmtString );  
   (void) vsnprintf( str, MAXSTRING, fmtString, args );
   va_end( args );

   output_if_debug("Mesa warning", str, GL_TRUE);
}


/**
 * Report an internal implementation problem.
 * Prints the message to stderr via fprintf().
 *
 * \param ctx GL context.
 * \param fmtString problem description string.
 */
void
_mesa_problem( const GLcontext *ctx, const char *fmtString, ... )
{
   va_list args;
   char str[MAXSTRING];
   (void) ctx;

   va_start( args, fmtString );  
   vsnprintf( str, MAXSTRING, fmtString, args );
   va_end( args );

   _mesa_fprintf(stderr, "GLSL compiler implementation error: %s\n", str);
}

#ifdef _DEBUG
static const char *error_string( GLenum error )
{
   switch (error) {
   case GL_NO_ERROR:
      return "GL_NO_ERROR";
   case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";
   case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";
   case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";
   case GL_STACK_OVERFLOW:
      return "GL_STACK_OVERFLOW";
   case GL_STACK_UNDERFLOW:
      return "GL_STACK_UNDERFLOW";
   case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY";
   case GL_TABLE_TOO_LARGE:
      return "GL_TABLE_TOO_LARGE";
   case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
      return "GL_INVALID_FRAMEBUFFER_OPERATION";
   default:
      return "unknown";
   }
}
#endif

/**
 * Record an OpenGL state error.  These usually occur when the user
 * passes invalid parameters to a GL function.
 *
 * If debugging is enabled (either at compile-time via the DEBUG macro, or
 * run-time via the MESA_DEBUG environment variable), report the error with
 * _mesa_debug().
 * 
 * \param ctx the GL context.
 * \param error the error value.
 * \param fmtString printf() style format string, followed by optional args
 */
void
_mesa_error( GLcontext *ctx, GLenum error, const char *fmtString, ... )
{
#ifdef _DEBUG
      {
         char s[MAXSTRING], s2[MAXSTRING];
         va_list args;

         va_start(args, fmtString);
         vsnprintf(s, MAXSTRING, fmtString, args);
         va_end(args);

         _mesa_snprintf(s2, MAXSTRING, "%s in %s", error_string(error), s);
         output_if_debug("Mesa: User error", s2, GL_TRUE);
      }
#endif
}

/*@}*/
