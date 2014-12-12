#include <core/exception.h>
#include "zip_file.h"

/* External functions */

extern void fill_string_filefunc(zlib_filefunc_def * p);


/* ZipFile methods */

//
// Constructor
//
ZipFile::ZipFile(const QString & path)
	: ExtIODevice(path)
{
	fill_string_filefunc(&m_ZipFunc);
	m_ZipFile = unzOpen2("packed", &m_ZipFunc);
	if (unlikely(!m_ZipFile))
		throw Exception("ZIP not initialized.");

	try
	{
		if (unzLocateFile(m_ZipFile, path.toLocal8Bit().constData(), 1) != UNZ_OK)
			throw Exception(QString("File not found: \"%1\"").arg(path));

		unz_file_info info;
		memset(&info, 0, sizeof(info));
		unzGetCurrentFileInfo(m_ZipFile, &info, NULL, 0, NULL, 0, NULL, 0);
		m_FileSize = info.uncompressed_size;

		unzOpenCurrentFile(m_ZipFile);
	}
	catch (...)
	{
		unzClose(m_ZipFile);
		throw;
	}	

#ifdef _PG_ENABLE_QT4
	setOpenMode(QIODevice::ReadOnly);
#endif
}

//
// Destructor
//
ZipFile::~ZipFile()
{
	unzClose(m_ZipFile);
}

//
// Closes the file
//
void ZipFile::close()
{
	unzCloseCurrentFile(m_ZipFile);
#ifdef _PG_ENABLE_QT4
	setOpenMode(QIODevice::NotOpen);
#endif
}

//
// Sets current position in the file
//
bool ZipFile::seek(qint64 pos)
{
	int64 p = ZipFile::pos();

	if (unlikely(p < 0))
		return false;

	if (pos < p)
	{
		unzCloseCurrentFile(m_ZipFile);
		unzOpenCurrentFile(m_ZipFile);

		p = 0;
	}

	int64 size = pos - p;
	while (size > 0)
	{
		char buf[1024];

		uint bytes = size > (int64)sizeof(buf) ? sizeof(buf) : (uint)size;
		if (unlikely(unzReadCurrentFile(m_ZipFile, buf, bytes) != bytes))
			return false;

		size -= bytes;
	}

	return true;
}

//
// Returns current position in the file
//
qint64 ZipFile::pos() const
{
	return unztell(m_ZipFile);
}

//
// Returns size of the file
//
qint64 ZipFile::size() const
{
	return m_FileSize;
}

//
// Opens the file
//
bool ZipFile::open(int mode)
{
	Q_ASSERT_MSG(false, "ZipFile::open() should never be called.");
	return true;
}

//
// Reads data from the device
//
qint64 ZipFile::readData(char * data, qint64 maxSize)
{
	int result = unzReadCurrentFile(m_ZipFile, data, (uint)maxSize);

	if (unlikely(result < 0))
		return -1;

	return result;
}

//
// Writes data from the device
//
qint64 ZipFile::writeData(const char * data, qint64 maxSize)
{
	return -1;
}
