//========================================================================
// GLFW - An OpenGL framework
// File:        thread.c
// Platform:    Any
// API version: 2.6
// WWW:         http://glfw.sourceforge.net
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Camilla Berglund
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include "internal.h"



//************************************************************************
//****                  GLFW internal functions                       ****
//************************************************************************

//========================================================================
// _glfwGetThreadPointer() - Find pointer to thread with a matching ID
//========================================================================

_GLFWthread * _glfwGetThreadPointer( int ID )
{
    _GLFWthread *t;

    for( t = &_glfwThrd.First; t != NULL; t = t->Next )
    {
        if( t->ID == ID )
        {
            break;
        }
    }

    return t;
}


//========================================================================
// _glfwAppendThread() - Append thread to thread list
//========================================================================

void _glfwAppendThread( _GLFWthread * t )
{
    _GLFWthread *t_tmp;

    t_tmp = &_glfwThrd.First;
    while( t_tmp->Next != NULL )
    {
        t_tmp = t_tmp->Next;
    }
    t_tmp->Next = t;
    t->Previous = t_tmp;
    t->Next     = NULL;
}


//========================================================================
// _glfwRemoveThread() - Remove thread from thread list
//========================================================================

void _glfwRemoveThread( _GLFWthread * t )
{
    if( t->Previous != NULL )
    {
        t->Previous->Next = t->Next;
    }
    if( t->Next != NULL )
    {
        t->Next->Previous = t->Previous;
    }
    free( (void *) t );
}



//************************************************************************
//****                     GLFW user functions                        ****
//************************************************************************

//========================================================================
// glfwCreateThread() - Create a new thread
//========================================================================

GLFWAPI GLFWthread GLFWAPIENTRY glfwCreateThread( GLFWthreadfun fun,
    void *arg )
{
    // Is GLFW initialized?
    if( !_glfwInitialized )
    {
        return -1;
    }

    // Return the GLFW thread ID
    return _glfwPlatformCreateThread( fun, arg );
}


//========================================================================
// glfwDestroyThread() - Kill a thread. NOTE: THIS IS A VERY DANGEROUS
// OPERATION, AND SHOULD NOT BE USED EXCEPT IN EXTREME SITUATIONS!
//========================================================================

GLFWAPI void GLFWAPIENTRY glfwDestroyThread( GLFWthread ID )
{
    // Is GLFW initialized?
    if( !_glfwInitialized )
    {
        return;
    }

    // Is it a valid thread? (killing the main thread is not allowed)
    if( ID < 1 )
    {
        return;
    }

    _glfwPlatformDestroyThread( ID );
}


//========================================================================
// glfwWaitThread() - Wait for a thread to die
//========================================================================

GLFWAPI int GLFWAPIENTRY glfwWaitThread( GLFWthread ID, int waitmode )
{
    // Is GLFW initialized?
    if( !_glfwInitialized )
    {
        return GL_TRUE;
    }

    // Is it a valid thread? (waiting for the main thread is not allowed)
    if( ID < 1 )
    {
        return GL_TRUE;
    }

    return _glfwPlatformWaitThread( ID, waitmode );
}


//========================================================================
// glfwGetThreadID() - Return the thread ID for the current thread
//========================================================================

GLFWAPI GLFWthread GLFWAPIENTRY glfwGetThreadID( void )
{
    // Is GLFW initialized?
    if( !_glfwInitialized )
    {
        return 0;
    }

    return _glfwPlatformGetThreadID();
}
