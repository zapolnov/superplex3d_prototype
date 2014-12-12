
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(glfw)

INCLUDE(${cmake_SOURCE_DIR}/common.cmake)
INCLUDE(${cmake_SOURCE_DIR}/defines.cmake)

SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${OPENGL_gl_LIBRARY})

INCLUDE(CheckCSourceCompiles)

IF(UNIX)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <X11/Xlib.h>
		#include <X11/extensions/Xrandr.h>

		int main() {; return 0;}
	"
		_GLFW_HAS_XRANDR
	)
	IF(_GLFW_HAS_XRANDR)
		ADD_DEFINITIONS(-D_GLFW_HAS_XRANDR)
		LINK_LIBRARIES(Xrandr)
	ENDIF(_GLFW_HAS_XRANDR)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <X11/Xlib.h>
		#include <X11/extensions/xf86vmode.h>

		#if defined(__APPLE_CC__)
		#error Not supported under Mac OS X
		#endif

		int main() {; return 0;}
	"
		_GLFW_HAS_XF86VIDMODE
	)
	IF(_GLFW_HAS_XF86VIDMODE)
		ADD_DEFINITIONS(-D_GLFW_HAS_XF86VIDMODE)
		LINK_LIBRARIES(Xxf86vm)
	ENDIF(_GLFW_HAS_XF86VIDMODE)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <X11/Xlib.h>
		#include <GL/glx.h>
		#include <GL/gl.h>
		int main(int argc, char **argv) {void *ptr=(void*)glXGetProcAddress(argv[0]); return 0;}
	"
		_GLFW_HAS_GLXGETPROCADDRESS
	)
	IF(_GLFW_HAS_GLXGETPROCADDRESS)
		ADD_DEFINITIONS(-D_GLFW_HAS_GLXGETPROCADDRESS)
	ENDIF(_GLFW_HAS_GLXGETPROCADDRESS)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <X11/Xlib.h>
		#include <GL/glx.h>
		#include <GL/gl.h>
		int main(int argc, char **argv) {void *ptr=(void*)glXGetProcAddressARB(argv[0]); return 0;}
	"
		_GLFW_HAS_GLXGETPROCADDRESSARB
	)
	IF(_GLFW_HAS_GLXGETPROCADDRESSARB)
		ADD_DEFINITIONS(-D_GLFW_HAS_GLXGETPROCADDRESSARB)
	ENDIF(_GLFW_HAS_GLXGETPROCADDRESSARB)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <X11/Xlib.h>
		#include <GL/glx.h>
		#include <GL/gl.h>
		int main(int argc, char **argv) {void *ptr=(void*)glXGetProcAddressEXT(argv[0]); return 0;}
	"
		_GLFW_HAS_GLXGETPROCADDRESSEXT
	)
	IF(_GLFW_HAS_GLXGETPROCADDRESSEXT)
		ADD_DEFINITIONS(-D_GLFW_HAS_GLXGETPROCADDRESSEXT)
	ENDIF(_GLFW_HAS_GLXGETPROCADDRESSEXT)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <dlfcn.h>
		int main(int argc, char **argv) {void *l=dlopen(argv[0],RTLD_LAZY|RTLD_GLOBAL); return 0;}
	"
		_GLFW_HAS_DLOPEN
	)
	IF(_GLFW_HAS_DLOPEN)
		ADD_DEFINITIONS(-D_GLFW_HAS_DLOPEN)
	ENDIF(_GLFW_HAS_DLOPEN)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <unistd.h>
		#ifndef _SC_NPROCESSORS_ONLN
		#ifndef _SC_NPROC_ONLN
		#error Neither _SC_NPROCESSORS_ONLN nor _SC_NPROC_ONLN available
		#endif
		#endif
		int main() {long x=sysconf(_SC_ARG_MAX); return 0; }
	"
		_GLFW_HAS_SYSCONF
	)
	IF(_GLFW_HAS_SYSCONF)
		ADD_DEFINITIONS(-D_GLFW_HAS_SYSCONF)
	ENDIF(_GLFW_HAS_SYSCONF)

	#################################################

	CHECK_C_SOURCE_COMPILES(
	"
		#include <sys/types.h>
		#include <sys/sysctl.h>
		#ifdef CTL_HW
		#ifdef HW_NCPU
		  yes;
		#endif
		#endif
		int main() { return 0; }
	"
		_GLFW_HAS_SYSCTL
	)
	IF(_GLFW_HAS_SYSCTL)
		ADD_DEFINITIONS(-D_GLFW_HAS_SYSCTL)
	ENDIF(_GLFW_HAS_SYSCTL)

ENDIF(UNIX)

#################################################

CHECK_C_SOURCE_COMPILES(
"
	#include <pthread.h>
	int main() {pthread_t posixID; posixID=pthread_self(); return 0;}
"
	_GLFW_HAS_PTHREAD
)
IF(_GLFW_HAS_PTHREAD)
	ADD_DEFINITIONS(-D_GLFW_HAS_PTHREAD)
ENDIF(_GLFW_HAS_PTHREAD)

#################################################

CHECK_C_SOURCE_COMPILES(
"
	#include <pthread.h>
	int main() {sched_yield(); return 0;}
"
	_GLFW_HAS_SCHED_YIELD
)
IF(_GLFW_HAS_SCHED_YIELD)
	ADD_DEFINITIONS(-D_GLFW_HAS_SCHED_YIELD)
ENDIF(_GLFW_HAS_SCHED_YIELD)

#################################################

SET(PLATFORM_FILES )

IF(WIN32)

	INCLUDE_DIRECTORIES(lib/win32)
	SET(PLATFORM_FILES
		lib/win32/platform.h
		lib/win32/win32_dllmain.c
		lib/win32/win32_enable.c
		lib/win32/win32_fullscreen.c
		lib/win32/win32_glext.c
		lib/win32/win32_init.c
		lib/win32/win32_joystick.c
		lib/win32/win32_thread.c
		lib/win32/win32_time.c
		lib/win32/win32_window.c
	)

ELSEIF(APPLE)

	INCLUDE_DIRECTORIES(lib/macosx)
	SET(PLATFORM_FILES
		lib/macosx/macosx_enable.c
		lib/macosx/macosx_fullscreen.c
		lib/macosx/macosx_glext.c
		lib/macosx/macosx_init.c
		lib/macosx/macosx_joystick.c
		lib/macosx/macosx_thread.c
		lib/macosx/macosx_time.c
		lib/macosx/macosx_window.c
		lib/macosx/platform.h
	)

ELSEIF(UNIX)

	INCLUDE_DIRECTORIES(lib/x11)
	SET(PLATFORM_FILES
		lib/x11/platform.h
		lib/x11/x11_enable.c
		lib/x11/x11_fullscreen.c
		lib/x11/x11_glext.c
		lib/x11/x11_init.c
		lib/x11/x11_joystick.c
		lib/x11/x11_keysym2unicode.c
		lib/x11/x11_thread.c
		lib/x11/x11_time.c
		lib/x11/x11_window.c
	)

ENDIF(WIN32)

INCLUDE_DIRECTORIES(lib include)

ADD_LIBRARY(glfw STATIC

	include/GL/glfw.h
	lib/enable.c
	lib/fullscreen.c
	lib/glext.c
	lib/image.c
	lib/init.c
	lib/input.c
	lib/internal.h
	lib/joystick.c
	lib/stream.c
	lib/tga.c
	lib/time.c
	lib/window.c

	${PLATFORM_FILES}

)

IF(APPLE)
	FIND_LIBRARY(Carbon Carbon)
	FIND_LIBRARY(AGL AGL)
	FIND_LIBRARY(AppKit AppKit)
	FIND_LIBRARY(ApplicationServices ApplicationServices)
	FIND_LIBRARY(CoreServices CoreServices)
	TARGET_LINK_LIBRARIES(glfw ${Carbon} ${AGL} ${AppKit} ${ApplicationServices} ${CoreServices})
ENDIF()
