#ifndef __INTERFACES__FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __INTERFACES__FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>

#ifdef _PG_ENABLE_QT4
 #include <core/application.h>
#endif

#include <core/ext_io_device.h>


/* Macros */

#ifdef _PG_ENABLE_QT4
 /** Returns pointer to the instance of the filesystem. */
 #define fileSystem() (Application::fileSystem())
#else
 #define fileSystem() (IFileSystem::instance())
#endif


/* Classes */

/** Interface to the file system. */
class GCC_PUBLIC IFileSystem
{
public:
	/** Returns path to the game data directory. */
	virtual const QString & dataPath() const = 0;

	/**
	 * Normalizes the specified path.
	 * This function removes duplicate slashes (//), this-level routes (/./) and up-level routes (/../)
	 * @param path Path to the directory.
	 */
	virtual QString normalizePath(const QString & path) const = 0;

	/**
	 * Returns contents of the specified file.
	 * @param path Path to the file.
	 */
	virtual QByteArray getFileContents(const QString & path) = 0;

	/**
	 * Opens specified file for reading.
	 * @param path Path to the file.
	 */
	virtual FilePtr openFile(const QString & path) = 0;

#ifndef _PG_ENABLE_QT4

	/** Returns pointer to the instance of the filesystem. */
	static inline IFileSystem * instance()
	{
		return m_Instance;
	}

protected:
	COREAPI static IFileSystem * m_Instance;		/**< Pointer to the instance of the filesystem. */

	/** Constructor. */
	inline IFileSystem()
	{
		Q_ASSERT(!m_Instance);
		m_Instance = this;
	}

#endif

protected:
	/** Destructor. */
	COREAPI virtual ~IFileSystem();
};

#endif // __INTERFACES__FILE_SYSTEM_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
