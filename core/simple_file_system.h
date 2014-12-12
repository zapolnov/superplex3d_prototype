#ifndef __CORE__SIMPLE_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__SIMPLE_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <interfaces/file_system.h>


/* Classes */

/** Basic implementation of the filesystem. */
class GCC_PUBLIC SimpleFileSystem : public IFileSystem
{
public:
	/**
	 * Constructor.
	 * @param path Path to the game data directory.
	 */
	COREAPI SimpleFileSystem(const QString & path);

	/** Destructor. */
	COREAPI ~SimpleFileSystem();

	/** Returns path to the game data directory. */
	COREAPI const QString & dataPath() const;

	/**
	 * Normalizes the specified path.
	 * This function removes duplicate slashes (//), this-level routes (/./) and up-level routes (/../)
	 * @param path Path to the directory.
	 */
	COREAPI QString normalizePath(const QString & path) const;

	/**
	 * Returns contents of the specified file.
	 * @param path Path to the file.
	 */
	COREAPI QByteArray getFileContents(const QString & path);

	/**
	 * Opens specified file for reading.
	 * @param path Path to the file.
	 */
	COREAPI FilePtr openFile(const QString & path);

private:
	QString m_BasePath;				/**< Base path to the game data directory. */
};

#endif // __CORE__SIMPLE_FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
