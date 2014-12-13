#ifndef __ENGINE__SYSTEM_H__
#define __ENGINE__SYSTEM_H__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <string>


/* Functions */

EXTERN_C_BEGIN

/** Initializes the operating system interface. */
void Sys_Init() throw(std::exception);

/** Shuts down the operating system interface. */
void Sys_Shutdown();

/**
 * Displays the specified error message.
 * @param text Error message text.
 */
ENGINEAPI void Sys_ErrorMessage(const std::string & text);

/** Returns path to the application executable file. */
ENGINEAPI const QString & Sys_GetAppPath();

/** Returns name of the application executable file. */
ENGINEAPI const QString & Sys_GetAppFile();

/** Updates the system timer. */
void Sys_UpdateTimer();

/** Returns current FPS. */
ENGINEAPI double Sys_GetCurrentFPS();

/** Returns time in seconds elapsed from the program start. */
ENGINEAPI double Sys_GetCurrentTime();

/** Returns time in seconds elapsed from the last frame. */
ENGINEAPI double Sys_GetLastFrameTime();

EXTERN_C_END

#endif // __ENGINE__SYSTEM_H__
