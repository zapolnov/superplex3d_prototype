#include <core/logger.h>
#include <core/stack_trace.h>
#include <core/atom.h>
#include <core/utility.h>
#include <ctype.h>
#include <string.h>

#ifdef _PG_ENABLE_QT4
 #include <QtCore/QStringList>
#endif

#ifdef WIN32
 #include <windows.h>
#endif


/* Private variables */

/** Names of the logging levels. */
static const char * g_LogLevels[] =
{
	"TRACE",
	"DEBUG",
	"NOTICE",
	"INFO",
	"WARN",
	"ERROR"
};



/* Public variables */

Logger logger;


/* StringFormatter methods */

NAMESPACE_INTERNAL_BEGIN

//
// Destructor
//
StringFormatter::~StringFormatter()
{
}

//
// Returns the formatted string
//
QString StringFormatter::getFormattedString() const
{
	std::vector<QVariant>::const_iterator it = m_Arguments.begin();
	int flags, field_width, precision, qualifier, base;
	QString target;

	target.clear();
	target.reserve(4096);

	// Process the string
	const QChar * fmt = m_Format.constData();
	const QChar * end = fmt + m_Format.size();
	for (; fmt < end; ++fmt)
	{
		// Copy the non-formatting characters
		if (likely(*fmt != L'%'))
		{
			target.append(*fmt);
			continue;
		}

		// Process flags
		flags = 0;
		for (;;)
		{
			switch (PG_UNICODE(++fmt))
			{
			case L'-':
				flags |= LEFT;
				continue;

			case L'+':
				flags |= PLUS;
				continue;

			case L' ':
				flags |= SPACE;
				continue;

			case L'#':
				flags |= SPECIAL;
				continue;

			case L'0':
				flags |= ZEROPAD;
				continue;
			}

			break;
		}

		// Get field width
		field_width = -1;
		if (isdigit(PG_UNICODE(fmt)))
			field_width = skipAtoI(fmt);
		else if (PG_UNICODE(fmt) == L'*')
		{
			Q_ASSERT(it != m_Arguments.end());

			++fmt;
			field_width = (it++)->toInt();
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// Get the precision
		precision = -1;
		if (*fmt == '.')
		{
			++fmt;
			if (isdigit(PG_UNICODE(fmt)))
				precision = skipAtoI(fmt);
			else if (PG_UNICODE(fmt) == L'*')
			{
				Q_ASSERT(it != m_Arguments.end());

				++fmt;
				precision = (it++)->toInt();
			}
			if (precision < 0)
				precision = 0;
		}

		// Get the conversion qualifier
		qualifier = 0;
		QChar ch = *fmt;
		if (ch == L'h' || ch == L'l' || ch == L'L')
		{
			qualifier = PG_UNICODE(&ch);
			++fmt;
		}

		// Default base
		base = 10;

		// Get the value
		Q_ASSERT(it != m_Arguments.end());
		const QVariant v = *it++;

		// Parse the format
		switch (PG_UNICODE(fmt))
		{
		// character
		case L'c':
			// Left padding
			if (!(flags & LEFT))
			{
				while (--field_width > 0)
					target.append(L' ');
			}

			// Put the character
			target.append(v.toChar());

			// Right padding
			while (--field_width > 0)
				target.append(L' ');
			continue;

		// string
		case L's':
			{
				const QString s = v.toString();
				const int len = s.length();

				// Left padding
				if (!(flags & LEFT))
				{
					while (len < field_width--)
						target.append(L' ');
				}

				// Put the string
				target.append(s);

				// Right padding
				while (len < field_width--)
					target.append(L' ');
			}
			continue;

		// Integer number formats - set up the flags and "break"
		case L'o':
			base = 8;
			break;

		case L'X':
			flags |= LARGE;
		case L'x':
			base = 16;
			break;

		case L'd':
		case L'i':
			flags |= SIGN;

		case L'u':
			break;

		case L'E':
		case L'e':
		case L'f':
		case L'G':
		case L'g':
			formatFloatingPoint<QString, wchar, double, wint_t, towlower, wcslen>
				(target, v.toDouble(), field_width, precision, PG_UNICODE(fmt), flags | SIGN);
			continue;

		default:
			if (*fmt != L'%')
				target.append(L'%');

			if (unlikely(fmt >= end))
				--fmt;
			else
				target.append(*fmt);

			continue;
		}

		// Parse the number
		formatNumber<QString, wchar, long long>
			(target, wLowerDigits, wUpperDigits, v.toLongLong(), base, field_width, precision, flags);
	}

	while (it != m_Arguments.end())
		target.append((it++)->toString());

	return target;
}


/* LoggerFormatter methods */

//
// Constructor
//
LoggerFormatter::LoggerFormatter()
{
}

//
// Constructor
//
LoggerFormatter::LoggerFormatter(const int level)
	: m_Level(level)
{
	m_Formatter = new StringFormatter(QString());
}

//
// Appends specified value to the formatter
//
void LoggerFormatter::append(const QVariant & val)
{
	Q_ASSERT(m_Formatter);

	if (m_Formatter->formatString().length() == 0)
		m_Formatter->setFormatString(val.toString());
	else	
		(*m_Formatter.get()) << val;
}

//
// Writes formatted message to the log
//
void LoggerFormatter::writeMessage()
{
	if (likely(m_Formatter->formatString().length() > 0))
	{
		QString message = m_Formatter->getFormattedString();
		logger.writeMessage(m_Level, message);
	}
}

//
// Pushes value to the formatter
//
LoggerFormatter & LoggerFormatter::operator<< (const StackTrace & value)
{
#ifdef _PG_ENABLE_QT4
	append(value.toByteArray());
#else
	append(value.toStdString());
#endif
	return *this;
}

#ifdef _PG_ENABLE_QT4

//
// Pushes value to the formatter
//
LoggerFormatter & LoggerFormatter::operator<< (const QStringList & value)
{
	append(value.join("\n"));
	return *this;
}

#endif // _PG_ENABLE_QT4

//
// Pushes value to the formatter
//
LoggerFormatter & LoggerFormatter::operator<< (const Atom & value)
{
	append(value.toString());
	return *this;
}

NAMESPACE_INTERNAL_END


/* Logger methods */

#ifndef _PG_ENABLE_QT4

//
// Emitted with each write to the log
//
void Logger::logMessageWritten(int level, const QByteArray & message)
{
}

#endif

//
// Initializes the logger
//
void Logger::init()
{
}

//
// Shuts down the logger
//
void Logger::shutdown()
{
}

//
// Processes the message and writes it into the log
//
void Logger::writeMessage(int level, const QString & message)
{
	const char * levelName;
	char buf[32];

	levelName = (unlikely(level < 0 || level >= (int)Q_ARRAY_SIZE(g_LogLevels))) ?
		"------" : g_LogLevels[level];

	snprintf(buf, sizeof(buf), "[%-6s] ", levelName);
	QByteArray prefix = buf;

	QByteArray msg = message.toLocal8Bit();
	const char * start = msg.constData();
	const char * p;

	tbb::mutex::scoped_lock lock(m_LogMutex);
	for (;;)
	{
		p = strchr(start, '\n');
		if (!p)
		{
			outputLogMessage(level, prefix, start);
    		break;
		}

		outputLogMessage(level, prefix, QByteArray(start, p - start));
		start = p + 1;
	}
}

//
// Writes processed message into the log
//
void Logger::outputLogMessage(int level, const QByteArray & prefix, const QByteArray & message)
{
	QByteArray msg = prefix + message + "\n";

	// Send the signal
	emit logMessageWritten(level, message);

	// Write message to the debugger
#ifdef WIN32
 #ifdef _DEBUG
	OutputDebugStringA(msg.constData());
 #endif
#endif
	fputs(msg.constData(), stderr);
}
