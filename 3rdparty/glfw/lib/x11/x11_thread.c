//========================================================================
// GLFW - An OpenGL framework
// File:        x11_thread.c
// Platform:    X11 (Unix)
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

#ifdef _GLFW_HAS_PTHREAD

//========================================================================
// _glfwNewThread() - This is simply a "wrapper" for calling the user
// thread function.
//========================================================================

void * _glfwNewThread( void * arg )
{
    GLFWthreadfun threadfun;
    _GLFWthread   *t;
    pthread_t   posixID;

    // Get current thread ID
    posixID = pthread_self();

    // Enter critical section
    ENTER_THREAD_CRITICAL_SECTION

    // Loop through entire list of threads to find the matching POSIX
    // thread ID
    for( t = &_glfwThrd.First; t != NULL; t = t->Next )
    {
        if( t->PosixID == posixID )
        {
            break;
        }
    }
    if( t == NULL )
    {
        LEAVE_THREAD_CRITICAL_SECTION
        return NULL;
    }

    // Get user thread function pointer
    threadfun = t->Function;

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Call the user thread function
    threadfun( arg );

    // Remove thread from thread list
    ENTER_THREAD_CRITICAL_SECTION
    _glfwRemoveThread( t );
    LEAVE_THREAD_CRITICAL_SECTION

    // When the thread function returns, the thread will die...
    return NULL;
}

#endif // _GLFW_HAS_PTHREAD



//************************************************************************
//****               Platform implementation functions                ****
//************************************************************************

//========================================================================
// _glfwPlatformCreateThread() - Create a new thread
//========================================================================

GLFWthread _glfwPlatformCreateThread( GLFWthreadfun fun, void *arg )
{
#ifdef _GLFW_HAS_PTHREAD

    GLFWthread  ID;
    _GLFWthread *t;
    int         result;

    // Enter critical section
    ENTER_THREAD_CRITICAL_SECTION

    // Create a new thread information memory area
    t = (_GLFWthread *) malloc( sizeof(_GLFWthread) );
    if( t == NULL )
    {
        // Leave critical section
        LEAVE_THREAD_CRITICAL_SECTION
        return -1;
    }

    // Get a new unique thread id
    ID = _glfwThrd.NextID ++;

    // Store thread information in the thread list
    t->Function = fun;
    t->ID       = ID;

    // Create thread
    result = pthread_create(
        &t->PosixID,      // Thread handle
        NULL,             // Default thread attributes
        _glfwNewThread,   // Thread function (a wrapper function)
        (void *)arg       // Argument to thread is user argument
    );

    // Did the thread creation fail?
    if( result != 0 )
    {
        free( (void *) t );
        LEAVE_THREAD_CRITICAL_SECTION
        return -1;
    }

    // Append thread to thread list
    _glfwAppendThread( t );

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Return the GLFW thread ID
    return ID;

#else

    return -1;

#endif // _GLFW_HAS_PTHREAD
}


//========================================================================
// _glfwPlatformDestroyThread() - Kill a thread. NOTE: THIS IS A VERY
// DANGEROUS OPERATION, AND SHOULD NOT BE USED EXCEPT IN EXTREME
// SITUATIONS!
//========================================================================

void _glfwPlatformDestroyThread( GLFWthread ID )
{
#ifdef _GLFW_HAS_PTHREAD

    _GLFWthread *t;

    // Enter critical section
    ENTER_THREAD_CRITICAL_SECTION

    // Get thread information pointer
    t = _glfwGetThreadPointer( ID );
    if( t == NULL )
    {
        LEAVE_THREAD_CRITICAL_SECTION
        return;
    }

    // Simply murder the process, no mercy!
    pthread_kill( t->PosixID, SIGKILL );

    // Remove thread from thread list
    _glfwRemoveThread( t );

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

#endif // _GLFW_HAS_PTHREAD
}


//========================================================================
// _glfwPlatformWaitThread() - Wait for a thread to die
//========================================================================

int _glfwPlatformWaitThread( GLFWthread ID, int waitmode )
{
#ifdef _GLFW_HAS_PTHREAD

    pthread_t   thread;
    _GLFWthread *t;

    // Enter critical section
    ENTER_THREAD_CRITICAL_SECTION

    // Get thread information pointer
    t = _glfwGetThreadPointer( ID );

    // Is the thread already dead?
    if( t == NULL )
    {
        LEAVE_THREAD_CRITICAL_SECTION
        return GL_TRUE;
    }

    // If got this far, the thread is alive => polling returns FALSE
    if( waitmode == GLFW_NOWAIT )
    {
        LEAVE_THREAD_CRITICAL_SECTION
        return GL_FALSE;
    }

    // Get thread handle
    thread = t->PosixID;

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Wait for thread to die
    (void) pthread_join( thread, NULL );

    return GL_TRUE;

#else

    return GL_TRUE;

#endif // _GLFW_HAS_PTHREAD
}


//========================================================================
// _glfwPlatformGetThreadID() - Return the thread ID for the current
// thread
//========================================================================

GLFWthread _glfwPlatformGetThreadID( void )
{
#ifdef _GLFW_HAS_PTHREAD

    _GLFWthread *t;
    GLFWthread  ID = -1;
    pthread_t   posixID;

    // Get current thread ID
    posixID = pthread_self();

    // Enter critical section
    ENTER_THREAD_CRITICAL_SECTION

    // Loop through entire list of threads to find the matching POSIX
    // thread ID
    for( t = &_glfwThrd.First; t != NULL; t = t->Next )
    {
        if( t->PosixID == posixID )
        {
            ID = t->ID;
            break;
        }
    }

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Return the found GLFW thread identifier
    return ID;

#else

    return 0;

#endif // _GLFW_HAS_PTHREAD
}
