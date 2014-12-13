
cmake_minimum_required(VERSION 2.8)

PROJECT(glfw C CXX)

DISABLE_WARNINGS()

if (WIN32)
    option(GLFW_USE_DWM_SWAP_INTERVAL "Set swap interval even when DWM compositing is enabled" OFF)
    option(GLFW_USE_OPTIMUS_HPG "Force use of high-performance GPU on Optimus systems" OFF)
endif()

if (APPLE)
    option(GLFW_USE_CHDIR "Make glfwInit chdir to Contents/Resources" ON)
    option(GLFW_USE_MENUBAR "Populate the menu bar on first window creation" ON)
else()
    option(GLFW_USE_EGL "Use EGL for context creation" OFF)
endif()

if (GLFW_USE_EGL)
    set(GLFW_CLIENT_LIBRARY "opengl" CACHE STRING
        "The client library to use; one of opengl, glesv1 or glesv2")

    if (${GLFW_CLIENT_LIBRARY} STREQUAL "opengl")
        set(_GLFW_USE_OPENGL 1)
    elseif (${GLFW_CLIENT_LIBRARY} STREQUAL "glesv1")
        set(_GLFW_USE_GLESV1 1)
    elseif (${GLFW_CLIENT_LIBRARY} STREQUAL "glesv2")
        set(_GLFW_USE_GLESV2 1)
    else()
        message(FATAL_ERROR "Unsupported client library")
    endif()

    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    find_package(EGL REQUIRED)
else()
    set(_GLFW_USE_OPENGL 1)
endif()

if (_GLFW_USE_OPENGL)
    find_package(OpenGL REQUIRED)
elseif (_GLFW_USE_GLESV1)
    find_package(GLESv1 REQUIRED)
elseif (_GLFW_USE_GLESV2)
    find_package(GLESv2 REQUIRED)
endif()

find_package(Threads REQUIRED)

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#--------------------------------------------------------------------
# Detect and select backend APIs
#--------------------------------------------------------------------
if (WIN32)
    set(_GLFW_WIN32 1)
    message(STATUS "Using Win32 for window creation") 

    if (GLFW_USE_EGL)
        set(_GLFW_EGL 1)
        message(STATUS "Using EGL for context creation")
    else()
        set(_GLFW_WGL 1)
        message(STATUS "Using WGL for context creation")
    endif()
elseif (APPLE)
    set(_GLFW_COCOA 1)
    message(STATUS "Using Cocoa for window creation")
    set(_GLFW_NSGL 1)
    message(STATUS "Using NSGL for context creation")
elseif (UNIX)
    set(_GLFW_X11 1)
    message(STATUS "Using X11 for window creation") 

    if (GLFW_USE_EGL)
        set(_GLFW_EGL 1)
        message(STATUS "Using EGL for context creation")
    else()
        set(_GLFW_GLX 1)
        message(STATUS "Using GLX for context creation")
    endif()
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()

#--------------------------------------------------------------------
# Use Win32 for window creation
#--------------------------------------------------------------------
if (_GLFW_WIN32)
    if (GLFW_USE_DWM_SWAP_INTERVAL)
        set(_GLFW_USE_DWM_SWAP_INTERVAL 1)
    endif()
    if (GLFW_USE_OPTIMUS_HPG)
        set(_GLFW_USE_OPTIMUS_HPG 1)
    endif()

    # HACK: When building on MinGW, WINVER and UNICODE need to be defined before
    # the inclusion of stddef.h (by glfw3.h), which is itself included before
    # win32_platform.h.  We define them here until a saner solution can be found
    # NOTE: MinGW-w64 and Visual C++ do /not/ need this hack.
    add_definitions(-DUNICODE)
    add_definitions(-DWINVER=0x0501)
endif()

#--------------------------------------------------------------------
# Use WGL for context creation
#--------------------------------------------------------------------
if (_GLFW_WGL)
    list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND glfw_LIBRARIES ${OPENGL_gl_LIBRARY})
endif()

#--------------------------------------------------------------------
# Use X11 for window creation
#--------------------------------------------------------------------
if (_GLFW_X11)

    find_package(X11 REQUIRED)

    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} x11")

    # Set up library and include paths
    list(APPEND glfw_INCLUDE_DIRS ${X11_X11_INCLUDE_PATH})
    list(APPEND glfw_LIBRARIES ${X11_X11_LIB} ${CMAKE_THREAD_LIBS_INIT})
    if (UNIX AND NOT APPLE)
        list(APPEND glfw_LIBRARIES ${RT_LIBRARY})
    endif()

    # Check for XRandR (modern resolution switching and gamma control)
    if (NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "The RandR library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_Xrandr_INCLUDE_PATH})
    list(APPEND glfw_LIBRARIES ${X11_Xrandr_LIB})
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xrandr")

    # Check for XInput (high-resolution cursor motion)
    if (NOT X11_Xinput_FOUND)
        message(FATAL_ERROR "The XInput library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_Xinput_INCLUDE_PATH})

    if (X11_Xinput_LIB)
        list(APPEND glfw_LIBRARIES ${X11_Xinput_LIB})
    else()
        # Backwards compatibility (bug in CMake 2.8.7)
        list(APPEND glfw_LIBRARIES Xi)
    endif()
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xi")

    # Check for Xf86VidMode (fallback gamma control)
    if (NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "The Xf86VidMode library and headers were not found")
    endif()

    list(APPEND glfw_INCLUDE_DIRS ${X11_xf86vmode_INCLUDE_PATH})
    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} xxf86vm")

    if (X11_Xxf86vm_LIB)
        list(APPEND glfw_LIBRARIES ${X11_Xxf86vm_LIB})
    else()
        # Backwards compatibility (see CMake bug 0006976)
        list(APPEND glfw_LIBRARIES Xxf86vm)
    endif()

    # Check for Xkb (X keyboard extension)
    if (NOT X11_Xkb_FOUND)
        message(FATAL_ERROR "The X keyboard extension headers were not found")
    endif() 

    list(APPEND glfw_INCLUDE_DIR ${X11_Xkb_INCLUDE_PATH})

    find_library(RT_LIBRARY rt)
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        list(APPEND glfw_LIBRARIES ${RT_LIBRARY})
    endif()

    find_library(MATH_LIBRARY m)
    mark_as_advanced(MATH_LIBRARY)
    if (MATH_LIBRARY)
        list(APPEND glfw_LIBRARIES ${MATH_LIBRARY})
    endif()

endif()

#--------------------------------------------------------------------
# Use GLX for context creation
#--------------------------------------------------------------------
if (_GLFW_GLX)

    list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND glfw_LIBRARIES ${OPENGL_gl_LIBRARY})

    set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} gl")

    include(CheckFunctionExists)

    set(CMAKE_REQUIRED_LIBRARIES ${OPENGL_gl_LIBRARY})
    check_function_exists(glXGetProcAddress _GLFW_HAS_GLXGETPROCADDRESS)
    check_function_exists(glXGetProcAddressARB _GLFW_HAS_GLXGETPROCADDRESSARB)
    check_function_exists(glXGetProcAddressEXT _GLFW_HAS_GLXGETPROCADDRESSEXT)

    if (NOT _GLFW_HAS_GLXGETPROCADDRESS AND
        NOT _GLFW_HAS_GLXGETPROCADDRESSARB AND
        NOT _GLFW_HAS_GLXGETPROCADDRESSEXT)
        message(WARNING "No glXGetProcAddressXXX variant found")

        # Check for dlopen support as a fallback

        set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS})
        check_function_exists(dlopen _GLFW_HAS_DLOPEN)
        if (NOT _GLFW_HAS_DLOPEN)
            message(FATAL_ERROR "No entry point retrieval mechanism found")
        endif()

        if (CMAKE_DL_LIBS)
            list(APPEND glfw_LIBRARIES ${CMAKE_DL_LIBS})
        endif()
    endif()

endif()

#--------------------------------------------------------------------
# Use EGL for context creation
#--------------------------------------------------------------------
if (_GLFW_EGL)

    list(APPEND glfw_INCLUDE_DIRS ${EGL_INCLUDE_DIR})
    list(APPEND glfw_LIBRARIES ${EGL_LIBRARY})

    if (UNIX)
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} egl")
    endif()

    if (_GLFW_USE_OPENGL)
        list(APPEND glfw_LIBRARIES ${OPENGL_gl_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} gl")
    elseif (_GLFW_USE_GLESV1)
        list(APPEND glfw_LIBRARIES ${GLESv1_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${GLESv1_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} glesv1_cm")
    elseif (_GLFW_USE_GLESV2)
        list(APPEND glfw_LIBRARIES ${GLESv2_LIBRARY})
        list(APPEND glfw_INCLUDE_DIRS ${GLESv2_INCLUDE_DIR})
        set(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} glesv2")
    endif()

endif()

#--------------------------------------------------------------------
# Use Cocoa for window creation and NSOpenGL for context creation
#--------------------------------------------------------------------
if (_GLFW_COCOA AND _GLFW_NSGL)
        
    if (GLFW_USE_MENUBAR)
        set(_GLFW_USE_MENUBAR 1)
    endif()

    if (GLFW_USE_CHDIR)
        set(_GLFW_USE_CHDIR 1)
    endif()

    # Set up library and include paths
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
    list(APPEND glfw_LIBRARIES ${COCOA_FRAMEWORK}
                               ${OPENGL_gl_LIBRARY}
                               ${IOKIT_FRAMEWORK}
                               ${CORE_FOUNDATION_FRAMEWORK}
                               ${CORE_VIDEO_FRAMEWORK})

    set(GLFW_PKG_DEPS "")
    list(APPEND glfw_LIBRARIES "-framework Cocoa")
    list(APPEND glfw_LIBRARIES "-framework OpenGL")
    list(APPEND glfw_LIBRARIES "-framework IOKit")
    list(APPEND glfw_LIBRARIES "-framework CoreFoundation")
    list(APPEND glfw_LIBRARIES "-framework CoreVideo")
endif()

#--------------------------------------------------------------------
# Export GLFW library dependencies
#--------------------------------------------------------------------
set(GLFW_LIBRARIES ${glfw_LIBRARIES} CACHE STRING "Dependencies of GLFW")

#--------------------------------------------------------------------
# Choose library output name
#--------------------------------------------------------------------
set(GLFW_LIB_NAME glfw)

#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
configure_file(${glfw_SOURCE_DIR}/src/glfw_config.h.in 
               ${glfw_BINARY_DIR}/src/glfw_config.h @ONLY)

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(src)
