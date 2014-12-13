#ifndef __CORE__LOGGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__LOGGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/pointer.h>
#include <core/variant.h>
#include <core/trolltech.h>
#include <vector>
#include <tbb/mutex.h>


/* Constants */

/** Low-level messages that dump state of a program. */
#define LOG_TRACE	0
/** Messages that can aid in debugging a problem. */
#define LOG_DEBUG	1
/** Informational messages with low priority. */
#define LOG_NOTICE	2
/** Standard informational messages about some process. */
#define LOG_INFO	3
/** Some non-critical problem was detected, program may continue execution. */
#define LOG_WARN	4
/** Problem that can break program execution. */
#define LOG_ERROR	5


/* Classes */

class StackTrace;

NAMESPACE_INTERNAL_BEGIN

/** Helper class for string formatting. */
class StringFormatter : public RefCounted
{
public:
	/** Default constructor. */
	inline StringFormatter()
	{
	}

	/**
	 * Constructor.
	 * @param fmt Format string.
	 */
	inline StringFormatter(const QString & fmt)
	{
		m_Format = fmt;
	}

	/** Destructor. */
	COREAPI ~StringFormatter();

	/** Formats the string and returns the result. */
	COREAPI QString getFormattedString() const;

	/** Returns current format string. */
	inline const QString & formatString() const
	{
		return m_Format;
	}

	/**
	 * Sets the format string.
	 * @param fmt Format string.
	 */
	inline void setFormatString(const QString & fmt)
	{
		m_Format = fmt;
	}

	/**
	 * Pushes value to the formatter.
	 * @param value Value.
	 */
	inline StringFormatter & operator<< (const QVariant & value)
	{
		m_Arguments.push_back(value);
		return *this;
	}

private:
	QString m_Format;
	std::vector<QVariant> m_Arguments;
};

/** Helper class for Logger. */
class LoggerFormatter
{
public:
	/** Default constructor. */
	COREAPI LoggerFormatter();

	/**
	 * Constructor.
	 * @param level Logging level.
	 */
	COREAPI LoggerFormatter(const int level);

	/** Destructor. */
	inline ~LoggerFormatter()
	{
		writeMessage();
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const char value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const uchar value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const short value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const ushort value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const int value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const uint value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const long value)
	{
		append((qlonglong)value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const ulong value)
	{
		append((qulonglong)value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const longlong value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const ulonglong value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const float value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const double value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const long double value)
	{
		append((double)value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const bool value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const char * value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const wchar * value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const std::string & value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const std::wstring & value)
	{
		append(value);
		return *this;
	}

	/**
	 * Writes value into the formatter.
	 * @param value Value to write.
	 */
	inline LoggerFormatter & operator<< (const QVariant & value)
	{
		append(value);
		return *this;
	}

	/**
	 * Pushes value to the formatter.
	 * @param value Value.
	 */
	COREAPI LoggerFormatter & operator<< (const StackTrace & value);

private:
	Pointer<StringFormatter> m_Formatter;	/**< Pointer to the associated string formatter. */
	int m_Level;							/**< Logging level. */

	/**
	 * Appends specified value to the formatter.
	 * @param value Value to append.
	 */
	COREAPI void append(const QVariant & val);

	/** Writes formatter message to the log. */
	COREAPI void writeMessage();

	LoggerFormatter & operator= (const LoggerFormatter &);
};

NAMESPACE_INTERNAL_END

/** Logger. */
class Logger : public QObject
{
	Q_OBJECT

public:
	/**
	 * Initiates write of the log message.
	 * @param level Logging level.
	 */
	inline Internal::LoggerFormatter operator<< (const int level) const
	{
		return Internal::LoggerFormatter(level);
	}

signals:
	/**
	 * Emitted with each write to the log.
	 * @param level Level of the message.
	 * @param message Text of the message.
	 */
	void logMessageWritten(int level, const QByteArray & message);

private:
	tbb::mutex m_LogMutex;		/**< Mutex used for serialization of log writes. */

	/** Initializes the logger. */
	COREAPI void init();

	/** Shuts down the logger. */
	COREAPI void shutdown();

	/**
	 * Processes the message and writes it into the log.
	 * @param level Level of the message.
	 * @param message Text of the message.
	 */
	void writeMessage(int level, const QString & message);

	/**
	 * Writes processed message into the log.
	 * @param level Level of the message.
	 * @param prefix Prefix of the message.
	 * @param message Text of the message.
	 */
	void outputLogMessage(int level, const QByteArray & prefix, const QByteArray & message);

	friend class Application;
	friend class Internal::LoggerFormatter;
};


/* Variables */

COREAPI extern Logger logger;

#endif // __CORE__LOGGER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
