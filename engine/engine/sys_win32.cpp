#include <engine/system.h>

/* Private variables */

static QString g_AppPath;		/**< Path to the application. */
static QString g_AppFile;		/**< Name of the executable file. *


/* Public functions */

//
// OS-specific initialization
//
void Sys_InitOS()
{
	wchar_t buffer[MAX_PATH] = { 0 };

	// Get name of the executable file
	GetModuleFileNameW(NULL, buffer, sizeof(buffer));
	g_AppFile = std::wstring(buffer);

	// Split the filename
	wchar_t * i1 = wcsrchr(buffer, L'\\');
	wchar_t * i2 = wcsrchr(buffer, L'/');
	if (unlikely(!i1) || (i2 && i2 > i1))
		i1 = i2;
	if (unlikely(!i1))
		g_AppPath = ".";
	else
	{
		*i1 = 0;
		g_AppPath = std::wstring(buffer);
	}
}

//
// Displays the specified error message
//
void Sys_ErrorMessage(const std::string & text)
{
	MessageBoxA(NULL, text.c_str(), "Error",
		MB_ICONERROR | MB_OK | MB_TOPMOST | MB_SETFOREGROUND);
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
