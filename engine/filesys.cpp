#include <engine/filesys.h>
#include <engine/system.h>
#include "zip_file_system.h"
#include <errno.h>
#include <string.h>

/* Public functions */

//
// Initializes the filesystem
//
void FS_Init() throw(std::exception)
{
	new ZipFileSystem(Sys_GetAppPath());
}

//
// Shuts down the filesystem
//
void FS_Shutdown()
{
	delete safe_cast<ZipFileSystem *>(IFileSystem::instance());
}

//
// Reads contents of the specified file into the memory
//
std::string FS_GetFileContents(const std::string & name) throw(std::exception)
{
	return IFileSystem::instance()->getFileContents(name);
}
