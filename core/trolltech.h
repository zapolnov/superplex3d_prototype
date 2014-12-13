#ifndef __CORE__TROLLTECH_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__TROLLTECH_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>

#include <core/variant.h>
#include <core/unicode.h>
#include <vector>
#include <string.h>

#ifdef WIN32
 #include <windows.h>
#endif


/* Macros */

#ifndef CALLBACK
 #define CALLBACK
#endif

#define Q_DECLARE_METATYPE(x)

#define Q_OBJECT
#define Q_PROPERTY(x)

#define Q_DISABLE_COPY(x) \
	private: \
	x(const x &); \
	x & operator= (const x &);

#define signals protected
#define slots
#define emit


/* Types */

typedef longlong qlonglong;
typedef ulonglong qulonglong;
typedef int64 qint64;
typedef uint64 quint64;

typedef Variant QVariant;
typedef wchar_t QChar;


/* Classes */

/** Incomplete imitation of Qt's QPoint. */
class QPoint
{
public:
	/** Constructor. */
	inline QPoint()
	{
	}

	/**
	 * Constructor.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */
	inline QPoint(int x, int y)
		: m_X(x), m_Y(y)
	{
	}

	/** Returns X coordinate. */
	inline int x() const
	{
		return m_X;
	}

	/** Returns Y coordinate. */
	inline int y() const
	{
		return m_Y;
	}

	/** Returns reference to the X coordinate. */
	inline int & x()
	{
		return m_X;
	}

	/** Returns reference to the Y coordinate. */
	inline int & y()
	{
		return m_Y;
	}

	/**
	 * Comparison operator.
	 * @param pt Reference to the variable to compare with.
	 */
	inline bool operator==(const QPoint & pt) const
	{
		return (m_X == pt.m_X && m_Y == pt.m_Y);
	}

	/**
	 * Comparison operator.
	 * @param pt Reference to the variable to compare with.
	 */
	inline bool operator!=(const QPoint & pt) const
	{
		return (m_X != pt.m_X || m_Y != pt.m_Y);
	}

private:
	int m_X;		/**< X coordinate. */
	int m_Y;		/**< Y coordinate. */
};

/** Incomplete imitation of Qt's QSize. */
class QSize
{
public:
	/** Constructor. */
	inline QSize()
	{
	}

	/**
	 * Constructor.
	 * @param w Width.
	 * @param h Height.
	 */
	inline QSize(int w, int h)
		: m_Width(w), m_Height(h)
	{
	}

	/** Returns width. */
	inline int width() const
	{
		return m_Width;
	}

	/** Returns height. */
	inline int height() const
	{
		return m_Height;
	}

	/** Returns reference to the width. */
	inline int & width()
	{
		return m_Width;
	}

	/** Returns reference to the height. */
	inline int & height()
	{
		return m_Height;
	}

	/**
	 * Comparison operator.
	 * @param sz Reference to the variable to compare with.
	 */
	inline bool operator==(const QSize & sz) const
	{
		return (m_Width == sz.m_Width && m_Height == sz.m_Height);
	}

	/**
	 * Comparison operator.
	 * @param sz Reference to the variable to compare with.
	 */
	inline bool operator!=(const QSize & sz) const
	{
		return (m_Width != sz.m_Width || m_Height != sz.m_Height);
	}

private:
	int m_Width;		/**< Width. */
	int m_Height;		/**< Height. */
};

/** Incomplete imitation of Qt's QRect. */
class QRect
{
public:
	/** Constructor. */
	inline QRect()
		: m_TopLeft(0, 0), m_BottomRight(-1, -1)
	{
	}

	/**
	 * Constructor.
	 * @param topLeft Top left corner of the rectangle.
	 * @param bottomRight Bottom right corner of the rectangle.
	 */
	inline QRect(const QPoint & topLeft, const QPoint & bottomRight)
		: m_TopLeft(topLeft), m_BottomRight(bottomRight)
	{
	}

	/** Returns coordinates of the top left corner of the rectangle. */
	inline const QPoint & topLeft() const
	{
		return m_TopLeft;
	}

	/** Returns size of the rectangle. */
	inline QSize size() const
	{
		return QSize(m_BottomRight.x() - m_TopLeft.x() + 1, m_BottomRight.y() - m_TopLeft.y() + 1);
	}

private:
	QPoint m_TopLeft;		/**< Top left corner of the rectangle. */
	QPoint m_BottomRight;	/**< Bottom right corner of the rectangle. */
};

/** Incomplete imitation of Qt's QByteArray. */
class QByteArray : public std::string
{
public:
	/** Constructor. */
	inline QByteArray()
	{
	}

	/**
	 * Constructor.
	 * @param value Source value.
	 */
	inline QByteArray(const char * value)
		: std::string(value)
	{
	}

	/**
	 * Constructor.
	 * @param value Source string.
	 * @param len Length of the source string.
	 */
	inline QByteArray(const char * value, size_t len)
		: std::string(value, len)
	{
	}

	/**
	 * Constructor.
	 * @param len Length of the string.
	 * @param ch Character to fill array with.
	 */
	inline QByteArray(int size, char ch)
		: std::string(size, ch)
	{
	}

	/**
	 * Constructor.
	 * @param value Source value.
	 */
	inline QByteArray(const std::string & value)
		: std::string(value)
	{
	}

	/** Returns pointer to the data. */
	inline char * data()
	{
		return (char *)std::string::data();
	}

	/** Returns pointer to the data. */
	inline const char * data() const
	{
		return c_str();
	}

	/** Returns pointer to the string data. */
	inline const char * constData() const
	{
		return c_str();
	}
};

class QStringList;

/** Incomplete imitation of Qt's QString. */
class QString : public std::wstring
{
public:
	/** String split behaviour. */
	enum SplitBehavior
	{
		KeepEmptyParts = 0,		/**< If a field is empty, keep it in the result. */
		SkipEmptyParts			/**< If a field is empty, don't include it in the result. */
	};

	/** Constructor. */
	inline QString()
		: m_ArgumentIndex(0)
	{
	}

	/**
	 * Constructor from ASCII string.
	 * @param str ASCII string.
	 */
	COREAPI QString(const char * str);

	/**
	 * Constructor from ASCII string.
	 * @param str ASCII string.
	 */
	COREAPI QString(const std::string & str);

	/**
	 * Constructor.
	 * @param value Source value.
	 */
	inline QString(const std::wstring & value)
		: std::wstring(value), m_ArgumentIndex(0)
	{
	}

	/** Returns true if this string is empty. */
	inline bool isEmpty() const
	{
		return std::wstring::length() == 0;
	}

	/** Returns pointer to the string data. */
	inline const wchar_t * constData() const
	{
		return std::wstring::c_str();
	}

	/**
	 * Appends character to the end of the string.
	 * @param ch Character to append.
	 */
	inline void append(wchar_t ch)
	{
		std::wstring::push_back(ch);
	}

	/**
	 * Appends string to the end of this string.
	 * @param str String to append.
	 */
	inline void append(const std::wstring & str)
	{
		std::wstring::append(str);
	}

	/**
	 * Appends string to the end of this string.
	 * @param str String to append.
	 */
	inline void append(const char * str)
	{
		while (*str)
			std::wstring::push_back((wchar_t)(unsigned char)*str++);
	}

	/**
	 * Concatenates two strings.
	 * @param str Second string.
	 */
	inline QString operator+(const QString & str) const
	{
		QString result(*this);
		result.append(str);
		return result;
	}

	/**
	 * Returns true if this string starts with the specified substring.
	 * @param str Substring to check for.
	 */
	inline bool startsWith(const QString & str) const
	{
		size_t sublen = str.length();
		size_t len = length();
		return (len >= sublen && !memcmp(constData(), str.constData(), sublen * sizeof(value_type)));
	}

	/**
	 * Returns true if this string ends with the specified character.
	 * @param ch Character to check for.
	 */
	inline bool endsWith(QChar ch) const
	{
		size_t len = length();
		return (len > 0 && (*this)[len - 1] == ch);
	}

	/** Converts this string string into local 8-bit representation. */
	inline QByteArray toLocal8Bit() const
	{
		return convertWCHARtoLocal8Bit(*this);
	}

	/** Converts this string string into latin-1 representation. */
	COREAPI QByteArray toLatin1() const;

	/**
	 * Formats the string argument.
	 * @param a Value of the argument.
	 * @param fieldWidth Width of the field.
	 * @param fillChar Filler character.
	 */
	COREAPI QString arg(const QString & a, int fieldWidth = 0, const QChar & fillChar = L' ') const;

	/**
	 * Formats the integer argument.
	 * @param a Value of the argument.
	 * @param fieldWidth Width of the field.
	 * @param fillChar Filler character.
	 */
	COREAPI QString arg(uint a, int fieldWidth = 0, int base = 10, const QChar & fillChar = L' ') const;

	/**
	 * Creates string from ASCII string.
	 * @param str ASCII string.
	 */
	static inline QString fromLatin1(const char * str)
	{
		return QString(str);
	}

	/**
	 * Creates string from ASCII string.
	 * @param str ASCII string.
	 */
	static inline QString fromLatin1(const std::string & str)
	{
		return QString(str);
	}

	/**
	 * Creates string from local 8-bit string.
	 * @param str Local 8-bit string.
	 */
	static inline QString fromLocal8Bit(const char * str)
	{
		return convertLocal8BitToWCHAR(str, strlen(str));
	}

	/**
	 * Creates string from local 8-bit string.
	 * @param str Local 8-bit string.
	 */
	static inline QString fromLocal8Bit(const std::string & str)
	{
		return convertLocal8BitToWCHAR(str);
	}

	/**
	 * Comparison operator.
	 * @param str Second string.
	 */
	inline bool operator==(const QString & str) const
	{
		return static_cast<const std::wstring &>(*this) == static_cast<const std::wstring &>(str);
	}

	/**
	 * Comparison operator.
	 * @param str Second string.
	 */
	inline bool operator!=(const QString & str) const
	{
		return static_cast<const std::wstring &>(*this) != static_cast<const std::wstring &>(str);
	}

	/**
	 * Splits the string using the specified separator.
	 * @param sep Field separator.
	 * @param mode Split behavior.
	 */
	COREAPI QStringList split(const QChar sep, SplitBehavior mode = KeepEmptyParts) const;

private:
	int m_ArgumentIndex;		/**< Index of the current argument. */
};

/** Incomplete imitation of Qt's QStringList. */
class QStringList : public std::vector<QString>
{
public:
	/**
	 * Joins list elements with a glue string.
	 * @param glue Glue.
	 */
	COREAPI QString join(const QString & glue);

	/**
	 * Removes the specified item.
	 * @param idx Index of the item.
	 */
	inline void removeAt(int idx)
	{
		Q_ASSERT(idx >= 0 && idx < (int)size());
		erase(begin() + idx);
	}
};

/** Incomplete imitation of Qt's QObject. */
class QObject
{
public:
	/**
	 * Translates the specified string.
	 * @param str String to translate.
	 */
	static inline QString tr(const QString & str)
	{
		return str;
	}
};

/** Incomplete imitation of Qt's QIODevice. */
class GCC_PUBLIC QIODevice : public QObject
{
public:
	/** Device open mode. */
	enum OpenModeFlag
	{
		NotOpen = 0x00,			/**< Device is not open. */
		ReadOnly = 0x01,		/**< Read-only mode. */
		WriteOnly = 0x02,		/**< Write-only mode. */
		Truncate = 0x04,		/**< Truncate the device before opening. */
		Text = 0x08,			/**< Enable text I/O mode. */
	};

	/** Returns last error message. */
	inline QString errorString() const
	{
		return m_ErrorString;
	}

	/**
	 * Opens the device.
	 * @param mode Device open mode (@sa OpenModeFlag).
	 */
	COREAPI virtual bool open(int mode);

	/** Closes the device. */
	COREAPI virtual void close();

	/**
	 * Sets current position in the device.
	 * @param pos New position.
	 */
	COREAPI virtual bool seek(qint64 pos);

	/** Returns current position in the device. */
	COREAPI virtual qint64 pos() const;

	/** Returns size of the device. */
	COREAPI virtual qint64 size() const;

	/**
	 * Reads a line of character from the device.
	 * @param data Pointer to the destination buffer.
	 * @param maxSize Size of the buffer.
	 */
	inline qint64 readLine(char * data, qint64 maxSize)
	{
		Q_ASSERT(maxSize > 1);
		qint64 result = readLineData(data, maxSize);
		if (likely(result >= 0))
			data[result] = 0;
		return result;
	}

	/**
	 * Reads data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes read or -1 in case of error.
	 */
	inline qint64 read(char * data, qint64 maxSize)
	{
		return readData(data, maxSize);
	}

	/**
	 * Reads one character from the device.
	 * @param c Pointer to the destination variable.
	 */
	inline bool getChar(char * c)
	{
		char ch;
		if (!c)
			c = &ch;
		return read(c, 1) == 1;
	}

	/**
	 * Puts the character back into the device.
	 * @param c Character to put back.
	 */
	inline void ungetChar(char c)
	{
		qint64 p = pos();
		if (p > 0)
			seek(p - 1);
	}

protected:
	/**
	 * Reads data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes read or -1 in case of error.
	 */
	virtual qint64 readData(char * data, qint64 maxSize) = 0;

	/**
	 * Reads single line of characters from the device.
	 * @param data Pointer to the destination buffer.
	 * @param maxSize Size of the buffer.
	 */
	COREAPI virtual qint64 readLineData(char * data, qint64 maxSize);

	/**
	 * Sets error message.
	 * @param str Error string.
	 */
	inline void setErrorString(const QString & str)
	{
		m_ErrorString = str;
	}

private:
	QString m_ErrorString;		/**< Last error message. */
};

/** Incomplete imitation of Qt's QFile. */
class GCC_PUBLIC QFile : public QIODevice
{
public:
	/**
	 * Constructor.
     * @param name Name of the file.
	 */
	inline QFile(const QString & name)
		: m_FileName(name), m_Handle(NULL)
	{
	}

	/** Returns name of the file. */
	inline const QString & fileName() const
	{
		return m_FileName;
	}

	/**
	 * Opens the device.
	 * @param mode Device open mode (@sa OpenModeFlag).
	 */
	COREAPI bool open(int mode);

	/** Closes the device. */
	COREAPI void close();

	/** Returns true if file is currently open. */
	inline bool isOpen() const
	{
		return m_Handle != NULL;
	}

	/**
	 * Sets current position in the device.
	 * @param pos New position.
	 */
	COREAPI bool seek(qint64 pos);

	/** Returns current position in the device. */
	COREAPI qint64 pos() const;

	/** Returns size of the device. */
	COREAPI qint64 size() const;

protected:
	/**
	 * Reads data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes read or -1 in case of error.
	 */
	COREAPI qint64 readData(char * data, qint64 maxSize);

private:
	QString m_FileName;		/**< Name of the file. */
	FILE * m_Handle;		/**< Handle of the file. */
};

/** Incomplete imitation of Qt's QBuffer. */
class GCC_PUBLIC QBuffer : public QIODevice
{
public:
	/**
	 * Constructor.
	 * @param data Pointer to the buffer with data.
	 */
	inline QBuffer(QByteArray * data)
		: m_Data(data), m_Position(0), m_ReadAllowed(false), m_WriteAllowed(false)
	{
		Q_ASSERT(data);
	}

	/**
	 * Opens the device.
	 * @param mode Device open mode (@sa OpenModeFlag).
	 */
	COREAPI bool open(int mode);

	/** Closes the device. */
	COREAPI void close();

	/**
	 * Sets current position in the device.
	 * @param pos New position.
	 */
	COREAPI bool seek(qint64 pos);

	/** Returns current position in the device. */
	COREAPI qint64 pos() const;

	/** Returns size of the device. */
	COREAPI qint64 size() const;

	/** Provides access to the file data. */
	inline const QByteArray & buffer() const
	{
		Q_ASSERT(m_Data);
		return *m_Data;
	}

protected:
	/**
	 * Reads data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes read or -1 in case of error.
	 */
	COREAPI qint64 readData(char * data, qint64 maxSize);

	/**
	 * Reads single line of characters from the device.
	 * @param data Pointer to the destination buffer.
	 * @param maxSize Size of the buffer.
	 */
	COREAPI qint64 readLineData(char * data, qint64 maxSize);

private:
	QByteArray * m_Data;		/**< Pointer to the data. */
	qint64 m_Position;			/**< Current position in the file. */
	bool m_ReadAllowed : 1;		/**< If true, reading is allowed. */
	bool m_WriteAllowed : 1;	/**< If true, writing is allowed. */
};

/** Incomplete imitation of Qt's QFileInfo. */
class QFileInfo
{
public:
	/**
	 * Constructor.
	 * @param name Name of the file.
	 */
	inline QFileInfo(const QString & name)
		: m_FileName(name)
	{
	}

	/** Returns suffix of the file. */
	COREAPI QString suffix() const;

	/** Returns path to the file. */
	COREAPI QString absolutePath() const;

private:
	QString m_FileName;		/**< Name of the file. */
};

/** Incomplete imitation of Qt's QDir. */
class QDir
{
public:
	/**
	 * Converts path separators to OS native separators.
	 * @param path Path to process.
	 */
	COREAPI static QString toNativeSeparators(const QString & path);
};

/** Incomplete imitation of Qt's QLibrary. */
class QLibrary : public QObject
{
public:
	/**
	 * Constructor.
     * @param name Name of the library.
	 */
	inline QLibrary(const QString & name)
		: m_Name(name), m_Handle(NULL)
	{
	}

	/** Destructor. */
	COREAPI ~QLibrary();

	/** Returns name of the library. */
	inline const QString & fileName() const
	{
		return m_Name;
	}

	/** Returns last error message. */
	inline const QString & errorString() const
	{
		return m_ErrorString;
	}

	/**
	 * Loads the library.
	 * @returns true on success, false on failure.
	 */
	COREAPI bool load();

	/**
	 * Unloads the library.
	 * @returns true on success, false on failure.
	 */
	COREAPI bool unload();

	/**
	 * Returns pointer to the specified symbol.
	 * @param symbol Name of the symbol.
	 */
	COREAPI void * resolve(const char * symbol);

private:
	QString m_Name;			/**< Name of the library. */
	QString m_ErrorString;	/**< Error string. */
	void * m_Handle;		/**< Handle of the library. */
};

/** Incomplete imitation of Qt's QThread. */
class QThread : public QObject
{
public:
	/** Constructor. */
	COREAPI QThread();

	/** Destructor. */
	COREAPI ~QThread();

	/** Starts the thread. */
	COREAPI void start();

	/** Waits for completion of the thread. */
	COREAPI void wait();

protected:
	/** Thread body. */
	virtual void run() = 0;

private:
	void * m_Handle;		/**< Handle of the thread. */
	bool m_Running;			/**< True if thread is running. */

	/**
	 * Entry-point for the thread.
	 * @param param Pointer to the instance of QThread.
	 */
	static void * CALLBACK threadEntry(void * param);
};

#endif // __CORE__TROLLTECH_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
