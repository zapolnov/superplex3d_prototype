#ifndef __CORE__EXCEPTION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__EXCEPTION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <stdexcept>


/* Classes */

/** Exception class. */
class GCC_PUBLIC Exception : public std::runtime_error
{
public:
	/**
	 * Constructor.
	 * @param message Exception message.
	 */
	COREAPI Exception(const QString & message);

	/** Destructor. */
	COREAPI ~Exception() throw();
};

#endif // __CORE__EXCEPTION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
