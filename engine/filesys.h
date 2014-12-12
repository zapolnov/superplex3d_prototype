#ifndef __ENGINE__FILESYS_H__
#define __ENGINE__FILESYS_H__

/* Includes */

#include <core/common.h>
#include <string>


/* Macros */

#define FS_INIT_ZIP() \
	{ \
		extern unsigned long zipDataLength; \
		extern unsigned char zipData[]; \
		FS_InitZIP(zipData, zipDataLength); \
	}


/* Functions */

EXTERN_C_BEGIN

/** Initializes the filesystem. */
void FS_Init() throw(std::exception);

/** Shuts down the filesystem. */
void FS_Shutdown();

/**
 * Sets address of the ZIP file.
 * @param ptr Pointer to the data.
 * @param len Length of the data.
 */
ENGINEAPI void FS_InitZIP(const unsigned char * ptr, unsigned long len);

/**
 * Reads contents of the specified file into the memory.
 * @param name Name of the file.
 */
ENGINEAPI std::string FS_GetFileContents(const std::string & name) throw(std::exception);

EXTERN_C_END

#endif // __ENGINE__FILESYS_H__
