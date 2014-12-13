#ifndef __ENGINE__ZIP_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __ENGINE__ZIP_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/simple_file_system.h>
#include <core/trolltech.h>
#include <interfaces/file_system.h>


/* Classes */

/** ZIP-file based filesystem. */
class ZipFileSystem : public SimpleFileSystem
{
public:
	/**
	 * Constructor.
	 * @param path Path to the game data directory.
	 */
	ZipFileSystem(const QString & path);

	/** Destructor. */
	~ZipFileSystem();

	/**
	 * Returns contents of the specified file.
	 * @param path Path to the file.
	 */
	QByteArray getFileContents(const QString & path);

	/**
	 * Opens specified file for reading.
	 * @param path Path to the file.
	 */
	FilePtr openFile(const QString & path);
};

#endif // __ENGINE__ZIP_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
