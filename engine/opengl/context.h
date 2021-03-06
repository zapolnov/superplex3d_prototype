#ifndef __OPENGL__CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __OPENGL__CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>

#if defined __APPLE__
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
#elif defined WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
#endif

#ifndef __APPLE__
 #include <GL/gl.h>
 #include <GL/glu.h>
#endif

#include <opengl/glext.h>

#endif // __OPENGL__CONTEXT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
