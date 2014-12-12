#ifndef __CORE__SIGNAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__SIGNAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Classes */

/** Class used to send signals to other threads. */
class Signal
{
public:
	/** Constructor. */
	COREAPI Signal();

	/** Destructor. */
	COREAPI ~Signal();

	/** Emits the signal and wakes a single thread that is waiting for the signal. */
	COREAPI void signal();

	/**
	 * Waits for the signal to arrive.
	 * @param msecs Number of milliseconds to wait.
	 * @returns true if object was signalled, false on timeout.
	 */
	COREAPI bool wait(uint msecs);

protected:
	void * m_Handle;		/**< Handle of the signal. */
};

#endif // __CORE__SIGNAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
