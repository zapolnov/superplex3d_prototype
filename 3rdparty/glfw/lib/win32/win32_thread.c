//========================================================================
// GLFW - An OpenGL framework
// File:        win32_thread.c
// Platform:    Windows
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
// This is an implementation of POSIX "compatible" condition variables for
// Win32, as described by Douglas C. Schmidt and Irfan Pyarali:
// http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
//************************************************************************

enum {
    _GLFW_COND_SIGNAL     = 0,
    _GLFW_COND_BROADCAST  = 1
};

typedef struct {
    // Signal and broadcast event HANDLEs
    HANDLE events[ 2 ];

    // Count of the number of waiters
    unsigned int waiters_count;

    // Serialize access to <waiters_count>
    CRITICAL_SECTION waiters_count_lock;
} _GLFWcond;



//************************************************************************
//****                  GLFW internal functions                       ****
//************************************************************************

//========================================================================
// _glfwNewThread() - This is simply a "wrapper" for calling the user
// thread function.
//========================================================================

DWORD WINAPI _glfwNewThread( LPVOID lpParam )
{
    GLFWthreadfun threadfun;
    _GLFWthread   *t;

    // Get pointer to thread information for current thread
    t = _glfwGetThreadPointer( _glfwPlatformGetThreadID() );
    if( t == NULL )
    {
        return 0;
    }

    // Get user thread function pointer
    threadfun = t->Function;

    // Call the user thread function
    threadfun( (void *) lpParam );

    // Remove thread from thread list
    ENTER_THREAD_CRITICAL_SECTION
    _glfwRemoveThread( t );
    LEAVE_THREAD_CRITICAL_SECTION

    // When the thread function returns, the thread will die...
    return 0;
}



//************************************************************************
//****                     GLFW user functions                        ****
//************************************************************************

//========================================================================
// _glfwPlatformCreateThread() - Create a new thread
//========================================================================

GLFWthread _glfwPlatformCreateThread( GLFWthreadfun fun, void *arg )
{
    GLFWthread  ID;
    _GLFWthread *t, *t_tmp;
    HANDLE      hThread;
    DWORD       dwThreadId;

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
    hThread = CreateThread(
        NULL,              // Default security attributes
        0,                 // Default stack size (1 MB)
        _glfwNewThread,    // Thread function (a wrapper function)
        (LPVOID)arg,       // Argument to thread is the user argument
        0,                 // Default creation flags
        &dwThreadId        // Returned thread identifier
    );

    // Did the thread creation fail?
    if( hThread == NULL )
    {
        free( (void *) t );
        LEAVE_THREAD_CRITICAL_SECTION
        return -1;
    }

    // Store more thread information in the thread list
    t->Handle = hThread;
    t->WinID  = dwThreadId;

    // Append thread to thread list
    t_tmp = &_glfwThrd.First;
    while( t_tmp->Next != NULL )
    {
        t_tmp = t_tmp->Next;
    }
    t_tmp->Next = t;
    t->Previous = t_tmp;
    t->Next     = NULL;

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Return the GLFW thread ID
    return ID;
}


//========================================================================
// _glfwPlatformDestroyThread() - Kill a thread. NOTE: THIS IS A VERY
// DANGEROUS OPERATION, AND SHOULD NOT BE USED EXCEPT IN EXTREME
// SITUATIONS!
//========================================================================

void _glfwPlatformDestroyThread( GLFWthread ID )
{
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
    if( TerminateThread( t->Handle, 0 ) )
    {
        // Close thread handle
        CloseHandle( t->Handle );

        // Remove thread from thread list
        _glfwRemoveThread( t );
    }

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION
}


//========================================================================
// _glfwPlatformWaitThread() - Wait for a thread to die
//========================================================================

int _glfwPlatformWaitThread( GLFWthread ID, int waitmode )
{
    DWORD       result;
    HANDLE      hThread;
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

    // Get thread handle
    hThread = t->Handle;

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Wait for thread to die
    if( waitmode == GLFW_WAIT )
    {
        result = WaitForSingleObject( hThread, INFINITE );
    }
    else if( waitmode == GLFW_NOWAIT )
    {
        result = WaitForSingleObject( hThread, 0 );
    }
    else
    {
        return GL_FALSE;
    }

    // Did we have a time-out?
    if( result == WAIT_TIMEOUT )
    {
        return GL_FALSE;
    }
    return GL_TRUE;
}


//========================================================================
// _glfwPlatformGetThreadID() - Return the thread ID for the current
// thread
//========================================================================

GLFWthread _glfwPlatformGetThreadID( void )
{
    _GLFWthread *t;
    GLFWthread  ID = -1;
    DWORD       WinID;

    // Get Windows thread ID
    WinID = GetCurrentThreadId();

    // Enter critical section (to avoid an inconsistent thread list)
    ENTER_THREAD_CRITICAL_SECTION

    // Loop through entire list of threads to find the matching Windows
    // thread ID
    for( t = &_glfwThrd.First; t != NULL; t = t->Next )
    {
        if( t->WinID == WinID )
        {
            ID = t->ID;
            break;
        }
    }

    // Leave critical section
    LEAVE_THREAD_CRITICAL_SECTION

    // Return the found GLFW thread identifier
    return ID;
}
