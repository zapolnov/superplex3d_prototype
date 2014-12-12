#include <engine/system.h>
#include <core/logger.h>
#include <errno.h>

/* Private variables */

static QString g_AppPath;		/**< Path to the application. */
static QString g_AppFile;		/**< Name of the executable file. */


/* Public functions */

//
// OS-specific initialization
//
void Sys_InitOS()
{
	char link[256], buf[2048];

	// Get path to the executable file
	snprintf(link, sizeof(link), "/proc/%d/exe", getpid());
	ssize_t len = readlink(link, buf, sizeof(buf) - 1);
	if (unlikely(len < 0))
	{
		logger << LOG_ERROR << "readlink('%s') failed: %s" << link << strerror(errno);
		g_AppPath = ".";
		g_AppFile = "_unknown_";
	}
	else
	{
		buf[len] = 0;
		g_AppFile = QString::fromLocal8Bit(buf);

		// Split the filename
		char * p = strrchr(buf, '/');
		if (unlikely(!p))
			g_AppPath = ".";
		else
		{
			*p = 0;
			g_AppPath = QString::fromLocal8Bit(buf);
		}
	}
}

//
// Displays the specified error message
//
void Sys_ErrorMessage(const std::string & text)
{
	fprintf(stderr, "ERROR: %s\n", text.c_str());
}

//
// Returns path to the application executable file
//
const QString & Sys_GetAppPath()
{
	return g_AppPath;
}

//
// Returns name of the application executable file
//
const QString & Sys_GetAppFile()
{
	return g_AppFile;
}
