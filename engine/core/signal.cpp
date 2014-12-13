#include <core/signal.h>
#include <core/exception.h>

#ifdef WIN32
 #include <windows.h>
#endif

/* External functions */

#ifdef WIN32

extern "C" __declspec(dllimport)
std::string handle_win_error();

#endif


/* Signal methods */

//
// Constructor
//
Signal::Signal()
{
#ifdef WIN32
	m_Handle = CreateEvent(NULL, false, false, NULL);
	if (unlikely(!m_Handle))
		handle_win_error();
#else
	#warning Not implemented
#endif
}

//
// Destructor
//
Signal::~Signal()
{
#ifdef WIN32
	CloseHandle((HANDLE)m_Handle);
#else
	#warning Not implemented
#endif
}

//
// Emits the signal and wakes a single thread that is waiting for the signal
//
void Signal::signal()
{
#ifdef WIN32
	SetEvent((HANDLE)m_Handle);
#else
	#warning Not implemented
#endif
}

//
// Waits for the signal to arrive
//
bool Signal::wait(uint msecs)
{
#ifdef WIN32
	DWORD result = WaitForSingleObject((HANDLE)m_Handle, msecs);

	if (result == WAIT_OBJECT_0)
		return true;
	else if (result == WAIT_TIMEOUT)
		return false;
	else if (result == WAIT_ABANDONED)
		throw Exception("WaitForSingleObject() returned WAIT_ABANDONED");
	else
		handle_win_error();

	return false;
#else
	#warning Not implemented
	return false;
#endif
}
