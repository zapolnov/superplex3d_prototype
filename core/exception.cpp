#include <core/exception.h>
#include <core/logger.h>
#include <core/stack_trace.h>

/* Private functions */

//
// Converts QString into std::string
//
static
std::string toLocal8Bit(const QString & message)
{
#ifdef _PG_ENABLE_QT4
	QByteArray arr = message.toLocal8Bit();
	return std::string(arr.constData(), arr.length());
#else
	return convertWCHARtoLocal8Bit(message);
#endif
}


/* Exception methods */

//
// Constructor
//
Exception::Exception(const QString & message)
	: std::runtime_error(toLocal8Bit(message))
{
	logger << LOG_ERROR << "Exception thrown: %s\n%s" << message << StackTrace(true);
}

//
// Destructor
//
Exception::~Exception() throw()
{
}
