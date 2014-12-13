#include <core/exception.h>
#include <core/logger.h>
#include <core/simple_file_system.h>
#include <core/simple_file.h>
#include <core/trolltech.h>

/* SimpleFileSystem methods */

//
// Constructor
//
SimpleFileSystem::SimpleFileSystem(const QString & path)
	: m_BasePath(path)
{
	m_BasePath = normalizePath(m_BasePath);

	// Log path to the game data directory
	logger << LOG_INFO << "Game data directory: %s" << QDir::toNativeSeparators(m_BasePath);

	// Append trailing path delimiter
	if (!m_BasePath.endsWith('/')
#ifdef WIN32
			&& !m_BasePath.endsWith('\\')
#endif
		)
	{
		m_BasePath += '/';
	}
}

//
// Destructor
//
SimpleFileSystem::~SimpleFileSystem()
{
}

//
// Returns path to the game data directory
//
const QString & SimpleFileSystem::dataPath() const
{
	return m_BasePath;
}

//
// Normalizes the specified path
//
QString SimpleFileSystem::normalizePath(const QString & path) const
{
#ifdef WIN32
	QString p(path);
	for (QString::iterator it = p.begin(); it != p.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
	}
	QStringList list = p.split('/', QString::KeepEmptyParts);
#else
	QStringList list = path.split('/', QString::KeepEmptyParts);
#endif

	for (int i = 0; i < (int)list.size(); )
	{
		if (i > 0 && list[i].isEmpty())
			list.removeAt(i);
		else if (list[i] == "." && (list.size() > 2 || (list.size() == 2 && !list[i + 1].isEmpty())))
			list.removeAt(i);
		else if (i > 0 && list[i] == ".." && list[i - 1] != ".." && !list[i - 1].isEmpty())
		{
			list.removeAt(i);
			list.removeAt(--i);
		}
		else
			i++;
	}

	if (list.size() == 1 && list[0].isEmpty() && !path.isEmpty() && path[0] == '/')
		return "/";

	return list.join("/");
}

//
// Returns contents of the specified file
//
QByteArray SimpleFileSystem::getFileContents(const QString & path)
{
	QString fullPath = m_BasePath + path;

	// Open the file
 #ifdef WIN32
	FILE * f = _wfopen(fullPath.constData(), L"rb");
 #else
	FILE * f = fopen(fullPath.toLocal8Bit().constData(), "rb");
 #endif
	if (unlikely(!f))
		throw Exception(QObject::tr("Unable to open file \"%1\": %2").arg(fullPath).arg(strerror(errno)));

	// Read file contents
	QByteArray result;
	try
	{
		// Get file size
		if (unlikely(fseek(f, 0, SEEK_END) != 0))
			throw Exception(QObject::tr("Seek failed on file \"%1\": %2").arg(fullPath).arg(strerror(errno)));
		long size = ftell(f);
		if (unlikely(size < 0))
			throw Exception(QObject::tr("ftell() failed on file \"%1\": %2").arg(fullPath).arg(strerror(errno)));
		rewind(f);

		// Allocate memory
		result.resize(size);

		// Read the file
		long r = fread((char *)result.data(), 1, size, f);
		if (unlikely(r != size))
		{
			if (ferror(f))
				throw Exception(QObject::tr("Read failed in file \"%1\": %2").arg(fullPath).arg(strerror(errno)));
			else
				throw Exception(QObject::tr("Incomplete read in file \"%1\"").arg(fullPath));
		}
	}
	catch (...)
	{
		fclose(f);
		throw;
	}

	// Close the file
	fclose(f);

	return result;
}

//
// Opens specified file for reading
//
FilePtr SimpleFileSystem::openFile(const QString & path)
{
	return new SimpleFile(path);
}
