#include <core/trolltech.h>
#include <core/exception.h>
#include <core/logger.h>
#include <interfaces/file_system.h>
#include <errno.h>
#include <stdio.h>

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
#else
 #include <dlfcn.h>
#endif


/* External functions */

#ifdef WIN32

extern "C" __declspec(dllimport)
void handle_win_error();

extern "C" __declspec(dllimport)
std::string decode_win_error(int error_code);

#endif


/* QString methods */

//
// Constructor from ASCII string
//
QString::QString(const char * str)
	: std::wstring(strlen(str), L' '), m_ArgumentIndex(0)
{
	std::wstring::iterator it = begin();
	while (*str)
		*it++ = (unsigned char)*str++;
}

//
// Constructor from ASCII string
//
QString::QString(const std::string & str)
	: std::wstring(str.length(), L' '), m_ArgumentIndex(0)
{
	std::string::const_iterator it = str.begin();
	std::wstring::iterator jt = begin();
	while (it != str.end())
		*jt++ = (unsigned char)*it++;
}

//
// Converts this string string into latin-1 representation
//
QByteArray QString::toLatin1() const
{
	QByteArray result;

	result.reserve(std::wstring::length());
	for (std::wstring::const_iterator it = begin(); it != end(); it++)
	{
		wchar_t wc = *it;
		result.push_back((wc >= 0 && wc < 128) ? wc : '?');
	}

	return result;
}

//
// Formats the string argument
//
QString QString::arg(const QString & a, int fieldWidth, const QChar & fillChar) const
{
	QString result(*this);
	result.m_ArgumentIndex = m_ArgumentIndex + 1;

	wchar_t arg[10];
	swprintf(arg, wsizeof(arg), L"%%%d", result.m_ArgumentIndex);

	size_t sz = result.find(arg);
	if (sz != std::wstring::npos)
	{
		result.erase(sz, wcslen(arg));
		result.insert(sz, a);
	}

	return result;
}

//
// Formats the integer argument
//
QString QString::arg(uint a, int fieldWidth, int base, const QChar & fillChar) const
{
	QString result(*this);
	result.m_ArgumentIndex = m_ArgumentIndex + 1;

	wchar_t arg[10];
	swprintf(arg, wsizeof(arg), L"%%%d", result.m_ArgumentIndex);

	size_t sz = result.find(arg);
	if (sz != std::wstring::npos)
	{
		wchar_t value[32];
		swprintf(value, wsizeof(value), L"%u", a);

		result.erase(sz, wcslen(arg));
		result.insert(sz, value);
	}

	return result;
}

//
// Splits the string using the specified separator
//
QStringList QString::split(QChar sep, SplitBehavior mode) const
{
	QStringList list;
	QString field;

	for (const wchar_t * str = constData(); *str; str++)
	{
		if (*str != '/')
			field.append(*str);
		else
		{
			if (field.length() != 0 || mode == KeepEmptyParts)
				list.push_back(field);
			field.clear();
		}
	}

	if (field.length() != 0 || mode == KeepEmptyParts)
		list.push_back(field);

	return list;
}


/* QStringList methods */

//
// Joins list elements with a glue string
//
QString QStringList::join(const QString & glue)
{
	if (size() == 0)
		return QString();

	std::vector<QString>::const_iterator it = begin();
	QString result(*it++);
	for (; it != end(); ++it)
	{
		result.append(glue);
		result.append(*it);
	}

	return result;
}


/* QIODevice methods */

//
// Opens the device
//
bool QIODevice::open(int mode)
{
	return false;
}

//
// Closes the device
//
void QIODevice::close()
{
}

//
// Sets current position in the device
//
bool QIODevice::seek(qint64 pos)
{
	return false;
}

//
// Returns current position in the device
//
qint64 QIODevice::pos() const
{
	return 0;
}

//
// Returns size of the device
//
qint64 QIODevice::size() const
{
	return 0;
}

//
// Reads single line of characters from the device
//
qint64 QIODevice::readLineData(char * data, qint64 maxSize)
{
	uint bytesRead = 0;
	char ch = 0;

	while (maxSize > 1 && ch != '\n' && getChar(&ch))
	{
		bytesRead++;
		maxSize--;
		*data++ = ch;
	}

	return bytesRead;
}


/* QFile methods */

//
// Opens the device
//
bool QFile::open(int mode)
{
	bool truncate = (mode & Truncate) != 0;
	char mstr[16] = { 0 };

	close();

	switch (mode & (ReadOnly | WriteOnly))
	{
	case 0:
		setErrorString(QObject::tr("Invalid open mode."));
		return false;

	case ReadOnly:
		if (truncate)
		{
			setErrorString(QObject::tr("Invalid open mode."));
			return false;
		}
		strcpy(mstr, "r");
		break;

	case WriteOnly:
		strcpy(mstr, truncate ? "w" : "a");
		break;

	case ReadOnly | WriteOnly:
		strcpy(mstr, truncate ? "w+" : "a+");
		break;
	}

	if ((mode & Text) != 0)
		strcat(mstr, "t");
	else
		strcat(mstr, "b");

	if (m_FileName.length() == 0)
	{
		setErrorString(QObject::tr("Empty file name."));
		return false;
	}

	QByteArray name = (fileSystem()->dataPath() + m_FileName).toLocal8Bit();
	m_Handle = fopen(name.constData(), mstr);

	if (unlikely(m_Handle == NULL))
	{
		setErrorString(QString::fromLocal8Bit(strerror(errno)));
		return false;
	}

	return true;
}

//
// Closes the device
//
void QFile::close()
{
	if (m_Handle)
	{
		fclose(m_Handle);
		m_Handle = NULL;
	}
}

//
// Sets current position in the device
//
bool QFile::seek(qint64 pos)
{
	if (unlikely(!m_Handle))
	{
		setErrorString(QObject::tr("File is not open."));
		return false;
	}

	if (unlikely(pos < LONG_MIN || pos > LONG_MAX))
	{
		setErrorString(QObject::tr("Invalid offset."));
		return false;
	}

	if (unlikely(fseek(m_Handle, (long)pos, SEEK_SET) != 0))
	{
		setErrorString(QString::fromLocal8Bit(strerror(errno)));
		return false;
	}

	return true;
}

//
// Returns current position in the device
//
qint64 QFile::pos() const
{
	Q_ASSERT(m_Handle);
	return ftell(m_Handle);
}

//
// Returns size of the device
//
qint64 QFile::size() const
{
	Q_ASSERT(m_Handle);

	long current = ftell(m_Handle);
	if (unlikely(current < 0))
	{
		logger << LOG_ERROR << "ftell() failed: %s" << strerror(errno);
		return 0;
	}

	if (unlikely(fseek(m_Handle, 0, SEEK_END) != 0))
	{
		logger << LOG_ERROR << "fseek() failed: %s" << strerror(errno);
		return 0;
	}

	qint64 size = ftell(m_Handle);
	if (unlikely(size < 0))
	{
		logger << LOG_ERROR << "ftell() failed: %s" << strerror(errno);
		size = 0;
	}

	if (unlikely(fseek(m_Handle, current, SEEK_SET) != 0))
		logger << LOG_ERROR << "fseek() failed: %s" << strerror(errno);

	return size;
}

//
// Reads data from the device
//
qint64 QFile::readData(char * data, qint64 maxSize)
{
	if (unlikely(!m_Handle))
	{
		setErrorString(QObject::tr("File is not open."));
		return -1;
	}

	if (unlikely(maxSize < 0 || maxSize > LONG_MAX))
	{
		setErrorString(QObject::tr("Bad request."));
		return false;
	}

	long off= ftell(m_Handle);

	long result = fread(data, 1, (size_t)maxSize, m_Handle);
	if (unlikely(result < 0))
	{
		setErrorString(QString::fromLocal8Bit(strerror(errno)));
		return -1;
	}

	return result;
}


/* QBuffer methods */

//
// Opens the device
//
bool QBuffer::open(int mode)
{
	Q_ASSERT((mode & (ReadOnly | WriteOnly)) != NotOpen);
	Q_ASSERT(m_Data);

	m_ReadAllowed = (mode & ReadOnly) != 0;
	m_WriteAllowed = (mode & WriteOnly) != 0;

	if (mode & Truncate)
		m_Data->clear();

	m_Position = 0;

	return true;
}

//
// Closes the device
//
void QBuffer::close()
{
	m_ReadAllowed = false;
	m_WriteAllowed = false;
}

//
// Sets current position in the device
//
bool QBuffer::seek(qint64 pos)
{
	if (unlikely(!m_ReadAllowed && !m_WriteAllowed))
		return false;
	m_Position = pos;
	return true;
}

//
// Returns current position in the device
//
qint64 QBuffer::pos() const
{
	if (unlikely(!m_ReadAllowed && !m_WriteAllowed))
		return 0;
	return m_Position;
}

//
// Returns size of the device
//
qint64 QBuffer::size() const
{
	if (unlikely(!m_ReadAllowed && !m_WriteAllowed))
		return 0;

	Q_ASSERT(m_Data);
	return m_Data->length();
}

//
// Reads data from the device
//
qint64 QBuffer::readData(char * data, qint64 maxSize)
{
	if (unlikely(!m_ReadAllowed))
	{
		setErrorString(tr("File is not open for reading."));
		return -1;
	}

	Q_ASSERT(maxSize > 0);
	Q_ASSERT(m_Data);

	if (unlikely(m_Position < 0 || maxSize < 0))
	{
		setErrorString(tr("File position is negative."));
		return -1;
	}

	qint64 dataSize = (qint64)m_Data->size();
	if (unlikely(m_Position >= dataSize))
		return 0;

	if (m_Position + maxSize > dataSize)
		maxSize = dataSize - m_Position;

	memcpy(data, m_Data->constData() + m_Position, (size_t)maxSize);
	m_Position += maxSize;

	return maxSize;
}

//
// Reads single line of characters from the device
//
qint64 QBuffer::readLineData(char * data, qint64 maxSize)
{
	if (unlikely(!m_ReadAllowed))
	{
		setErrorString(tr("File is not open for reading."));
		return -1;
	}

	Q_ASSERT(maxSize > 1);
	Q_ASSERT(m_Data);

	if (unlikely(m_Position < 0 || maxSize < 0))
	{
		setErrorString(tr("File position is negative."));
		return -1;
	}

	qint64 dataSize = (qint64)m_Data->size();
	if (unlikely(m_Position >= dataSize))
	{
		setErrorString(tr("Read past end of file."));
		return -1;
	}

	qint64 bytesLeft = dataSize - m_Position;
	const char * src = m_Data->constData() + m_Position;
	--maxSize;

	qint64 bytes;
	char * ptr = (char *)memchr(src, '\n', (size_t)bytesLeft);
	if (!ptr)
		bytes = Q_MIN(bytesLeft, maxSize);
	else
	{
		size_t length = ptr - src + 1;
		bytes = Q_MIN(length, maxSize);
	}

	memcpy(data, src, (size_t)bytes);
	m_Position += bytes;

	return bytes;
}


/* QFileInfo methods */

//
// Returns suffix of the file
//
QString QFileInfo::suffix() const
{
	size_t dot = m_FileName.rfind(L'.');
	if (dot == QString::npos)
		return QString();

	size_t slash = m_FileName.rfind(L'/');
#ifdef WIN32
	size_t bslash = m_FileName.rfind(L'\\');
	if (bslash != QString::npos && (slash == QString::npos || bslash > slash))
		slash = bslash;
#endif

	if (slash != QString::npos && slash > dot)
		return QString();

	return m_FileName.substr(dot + 1);
}

//
// Returns path to the file
//
QString QFileInfo::absolutePath() const
{
	size_t slash = m_FileName.rfind(L'/');
#ifdef WIN32
	size_t bslash = m_FileName.rfind(L'\\');
	if (bslash != QString::npos && (slash == QString::npos || bslash > slash))
		slash = bslash;
#endif

	if (slash == QString::npos)
		return QString();

	return m_FileName.substr(0, slash);
}


/* QDir methods */

//
// Converts path separators to OS native separators
//
QString QDir::toNativeSeparators(const QString & path)
{
#ifdef WIN32
	QString result(path);
	for (QString::iterator it = result.begin(); it != result.end(); ++it)
	{
		if (*it == '/')
			*it = '\\';
	}
	return result;
#else
	return path;
#endif
}


/* QLibrary methods */

//
// Destructor
//
QLibrary::~QLibrary()
{
	unload();
}

//
// Loads the library
//
bool QLibrary::load()
{
#ifdef WIN32
	if (m_Handle)
		return true;

	m_Handle = LoadLibraryW(m_Name.c_str());
	if (unlikely(!m_Handle))
	{
		std::string msg = decode_win_error(GetLastError());
		m_ErrorString = QString("Unable to load library \"%1\": %2")
			.arg(m_Name).arg(QString::fromLocal8Bit(msg.c_str()));
		return false;
	}

	return true;
#else
	if (m_Handle)
		return true;

	m_Handle = dlopen(m_Name.toLocal8Bit().constData(), RTLD_GLOBAL);
	if (unlikely(!m_Handle))
	{
		std::string msg = dlerror();
		m_ErrorString = QString("Unable to load library \"%1\": %2")
			.arg(m_Name).arg(QString::fromLocal8Bit(msg.c_str()));
		return false;
	}

	return true;
#endif
}

//
// Unloads the library
//
bool QLibrary::unload()
{
#ifdef WIN32
	if (m_Handle)
	{
		FreeLibrary((HMODULE)m_Handle);
		m_Handle = NULL;
	}
	return true;
#else
	if (m_Handle)
	{
		dlclose(m_Handle);
		m_Handle = NULL;
	}
	return true;
#endif
}

//
// Returns pointer to the specified symbol
//
void * QLibrary::resolve(const char * symbol)
{
	if (!load())
		return NULL;

#ifdef WIN32
	Q_ASSERT(m_Handle);

	FARPROC func = GetProcAddress((HMODULE)m_Handle, symbol);
	if (unlikely(!func))
	{
		m_ErrorString = QString("Symbol %2 was not found in library \"%1\".")
			.arg(m_Name).arg(QString::fromLatin1(symbol));
	}

	return (void*)func;
#else
	Q_ASSERT(m_Handle);

	void * func = dlsym(m_Handle, symbol);
	if (unlikely(!func))
	{
		m_ErrorString = QString("Symbol %2 was not found in library \"%1\".")
			.arg(m_Name).arg(QString::fromLatin1(symbol));
	}

	return func;
#endif
}


/* QThread methods */

//
// Constructor
//
QThread::QThread()
	: m_Handle(NULL), m_Running(false)
{
}

//
// Destructor
//
QThread::~QThread()
{
#ifdef WIN32
	Q_ASSERT(!m_Running);
	if (m_Handle)
		CloseHandle(m_Handle);
#else
	if (m_Handle)
	{
		pthread_detach(*(pthread_t*)m_Handle);
		delete (pthread_t*)m_Handle;
	}
#endif
}

//
// Starts the thread
//
void QThread::start()
{
#ifdef WIN32
	if (m_Running)
		return;

	if (m_Handle)
		CloseHandle(m_Handle);

	m_Running = true;
	m_Handle = (HANDLE)_beginthreadex(NULL, 0, (unsigned (CALLBACK *)(void *))threadEntry, this, 0, NULL);
	if (unlikely(!m_Handle))
	{
		m_Running = false;
		throw Exception(QString("Unable to start thread: %1").arg(QString::fromLocal8Bit(strerror(errno))));
	}
#else
	if (m_Running)
		return;

	if (m_Handle)
	{
		pthread_detach(*(pthread_t*)m_Handle);
		delete (pthread_t*)m_Handle;
	}

	m_Handle = new pthread_t;
	m_Running = true;
	int err = pthread_create((pthread_t*)m_Handle, NULL, threadEntry, this);
	if (unlikely(err != 0))
	{
		m_Running = false;
		delete (pthread_t*)m_Handle;
		throw Exception(QString("Unable to start thread: %1").arg(QString::fromLocal8Bit(strerror(errno))));
	}
#endif
}

//
// Waits for completion of the thread
//
void QThread::wait()
{
#ifdef WIN32
	if (!m_Running)
		return;

	DWORD result = WaitForSingleObject((HANDLE)m_Handle, INFINITE);
	if (result == WAIT_OBJECT_0)
	{
		m_Running = false;
		return;
	}	
	else if (result == WAIT_TIMEOUT)
		logger << LOG_ERROR << "WaitForSingleObject() returned WAIT_TIMEOUT";
	else if (result == WAIT_ABANDONED)
		logger << LOG_ERROR << "WaitForSingleObject() returned WAIT_ABANDONED";
	else
	{
		logger << LOG_ERROR << "WaitForSingleObject(): %s"
			<< QString::fromLocal8Bit(decode_win_error(GetLastError()).c_str());
	}
#else
	if (!m_Running)
		return;

	int err = pthread_join(*(pthread_t*)m_Handle, NULL);
	if (err != 0)
	{
		logger << LOG_ERROR << "pthread_join: %s" << QString::fromLocal8Bit(strerror(errno));
		return;
	}

	m_Running = false;
	delete (pthread_t*)m_Handle;
	m_Handle = NULL;
#endif
}

//
// Entry-point for the thread
//
void * CALLBACK QThread::threadEntry(void * param)
{
	QThread * self = (QThread *)param;

	try
	{
		self->run();
	}
	catch (std::exception & e)
	{
		logger << LOG_ERROR << "%s" << QString::fromLocal8Bit(e.what());
	}
	catch (...)
	{
		logger << LOG_ERROR << "Unhandled exception in thread.";
	}

	self->m_Running = false;

	return NULL;
}
