#include <core/exception.h>
#include <core/logger.h>
#include <engine/filesys.h>
#include <zlib.h>
#include <contrib/minizip/ioapi.h>
#include <contrib/minizip/unzip.h>
#include "zip_file_system.h"
#include "zip_file.h"

/* External functions */

extern void fill_string_filefunc(zlib_filefunc_def * p);


/* ZipFileSystem methods */

//
// Constructor
//
ZipFileSystem::ZipFileSystem(const QString & path)
	: SimpleFileSystem(path)
{
}

//
// Destructor
//
ZipFileSystem::~ZipFileSystem()
{
}

//
// Returns contents of the specified file
//
QByteArray ZipFileSystem::getFileContents(const QString & path)
{
	zlib_filefunc_def zipFunc;
	unzFile zipFile = NULL;

	fill_string_filefunc(&zipFunc);
	zipFile = unzOpen2("packed", &zipFunc);

	if (unlikely(!zipFile))
		throw Exception("ZIP not initialized.");

	try
	{
		if (unzLocateFile(zipFile, path.toLocal8Bit().constData(), 1) != UNZ_OK)
			throw Exception(QString("File not found: \"%1\"").arg(path));

		unz_file_info info;
		memset(&info, 0, sizeof(info));
		unzGetCurrentFileInfo(zipFile, &info, NULL, 0, NULL, 0, NULL, 0);

		QByteArray result(info.uncompressed_size, 0);
		unzOpenCurrentFile(zipFile);
		unzReadCurrentFile(zipFile, (char *)result.data(), result.length());
		unzCloseCurrentFile(zipFile);

		unzClose(zipFile);
		zipFile = NULL;

		return result;
	}
	catch (...)
	{
		if (zipFile)
			unzClose(zipFile);
		throw;
	}	
}

//
// Opens specified file for reading
//
FilePtr ZipFileSystem::openFile(const QString & path)
{
	return new ZipFile(path);
}
