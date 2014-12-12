#include <engine/system.h>
#include <time.h>
#include <GL/glfw.h>
#include <core/logger.h>
#include <core/clock.h>
#include <stdlib.h>

/* Classes */

class Application
{
public:
	/** Initializes the logger. */
	static inline void initLogger()
	{
		logger.init();
		logger << LOG_INFO << "PolarGears casual games engine, version 1.0";
		logger << LOG_INFO << "Built on " __DATE__ " " __TIME__;
	}

	/** Shuts down the logger. */
	static inline void shutdownLogger()
	{
		logger.shutdown();
	}
};


/* Private variables */

static Clock * g_Clock = NULL;		/**< Pointer to the global clock. */
static double g_PrevTime = 0.0;		/**< Previous value of the timer. */


/* External functions */

extern void Sys_InitOS();


/* Public functions */

//
// Initializes the operating system interface
//
void Sys_Init() throw(std::exception)
{
	srand((unsigned int)time(NULL));

	Application::initLogger();
	Sys_InitOS();

	logger << LOG_TRACE << "Application path: %s" << Sys_GetAppPath();
	logger << LOG_TRACE << "Application executable: %s" << Sys_GetAppFile();

	if (unlikely(!glfwInit()))
		throw std::runtime_error("glfwInit() failed.");
	g_Clock = new Clock;
}

//
// Shuts down the operating system interface
//
void Sys_Shutdown()
{
	delete g_Clock;
	glfwTerminate();

	Application::shutdownLogger();
}

//
// Updates the system timer
//
void Sys_UpdateTimer()
{
	double time = glfwGetTime();
	g_Clock->update(time - g_PrevTime);
	g_PrevTime = time;
}

//
// Returns current FPS
//
double Sys_GetCurrentFPS()
{
	return g_Clock->getCurrentFPS();
}

//
// Returns time in seconds elapsed from the program start
//
double Sys_GetCurrentTime()
{
	return g_Clock->getCurrentTime();
}

//
// Returns time in seconds elapsed from the last frame
//
double Sys_GetLastFrameTime()
{
	return g_Clock->getLastFrameTime();
}
