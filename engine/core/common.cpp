#include <core/common.h>
#include <sstream>

/* Public functions */

NAMESPACE_INTERNAL_BEGIN

//
// Handles assertion failures
//
void Sys_AssertionFailed(const char * file, const int line,
	const char * function, const char * expr) throw(std::exception)
{
	std::ostringstream stream;

	stream << "Assertion \"" << expr << "\" failed in function " << function <<
		" (file \"" << file << "\", line " << line << ")";
	throw std::runtime_error(stream.str());
}

//
// Handles assertion failures
//
void Sys_AssertionFailedMsg(const char * file, const int line,
	const char * function, const char * msg) throw(std::exception)
{
	std::ostringstream stream;

	stream << msg << " in function " << function <<
		" (file \"" << file << "\", line " << line << ")";
	throw std::runtime_error(stream.str());
}

NAMESPACE_INTERNAL_END
