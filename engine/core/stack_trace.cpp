#include <core/stack_trace.h>
#ifdef __GNUC__
 #include <execinfo.h>
 #include <cxxabi.h>
#endif
#include <stdlib.h>

/* StackTrace methods */

//
// Constructor
//
StackTrace::StackTrace(bool getNames)
	: m_HaveNames(false)
{
#ifdef __GNUC__
	void * buffer[100];
	int nptrs = backtrace(buffer, sizeof(buffer) / sizeof(buffer[0]));

	m_Symbols.resize(nptrs);
	for (int i = 0; i < nptrs; i++)
		m_Symbols[i].address = buffer[i];

	if (getNames)
	{
		char ** strings = backtrace_symbols(buffer, nptrs);
		if (strings)
		{
			for (int i = 0; i < nptrs; i++)
				m_Symbols[i].name = strings[i];
			free(strings);
			demangleNames();
			m_HaveNames = true;
		}
	}
#endif // __GNUC__
}

//
// Destructor
//
StackTrace::~StackTrace()
{
}

//
// Gets names for functions
//
bool StackTrace::extractNames()
{
	if (m_HaveNames)
		return true;

#ifdef __GNUC__
	void * buffer[100];
	for (size_t i = 0; i < m_Symbols.size(); i++)
		buffer[i] = m_Symbols[i].address;

	char ** strings = backtrace_symbols(buffer, m_Symbols.size());
	if (!strings)
		return false;

	for (size_t i = 0; i < m_Symbols.size(); i++)
		m_Symbols[i].name = strings[i];

	free(strings);
	demangleNames();
	m_HaveNames = true;

	return true;
#endif // __GNUC__

#ifdef _MSC_VER
	return false;
#endif // _MSC_VER
}

//
// Converts this stack trace to the STL string
//
std::string StackTrace::toStdString() const
{
	std::string result, endl("\n");
	char buf[64];

	for (uint i = 0; i < m_Symbols.size(); i++)
	{
		snprintf(buf, sizeof(buf), "%016llX: ", (unsigned long long)((uintptr_t)m_Symbols[i].address));
		result.append(buf);
		result.append(m_Symbols[i].name.constData(), m_Symbols[i].name.length());
		if (i < m_Symbols.size() - 1)
			result.append(endl);
	}

	return result;
}

//
// Demangles function names
//
void StackTrace::demangleNames()
{
#ifdef __GNUC__
	for (size_t i = 0; i < m_Symbols.size(); i++)
	{
		const char * symbol = m_Symbols[i].name.constData();
		char * begin = 0, * end = 0;

		// Find the parentheses and address offset surrounding the mangled name
		for (const char * j = symbol; *j; ++j)
		{
			if (*j == '(')
				begin = const_cast<char *>(j);
			else if (*j == '+')
				end = const_cast<char *>(j);
		}

		if (begin && end)
		{
			size_t sz = 200;
			int status;

			*begin++ = 0;
			*end = 0;

			// Decode the mangled name
			char * function = (char *)malloc(sz);
			char * ret = abi::__cxa_demangle(begin, function, &sz, &status);
			if (ret)
				function = ret;
			else
			{
				// Demangling failed, just pretend it's a C function with no args
				strncpy(function, begin, sz);
				strncat(function, "()", sz);
				function[sz - 1] = 0;
			}
			m_Symbols[i].name = function;
			free(function);
		}
	}
#endif // __GNUC__
}
