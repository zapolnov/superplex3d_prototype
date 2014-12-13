#ifndef __INTERFACES__LOADABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __INTERFACES__LOADABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>


/* Classes */

class MemoryFile;

/** Interface to the data object that can be loaded from external source. */
class ILoadable : public RefCounted
{
public:
	/**
	 * Loads data from the specified memory file.
	 * @param file File data.
	 */
	virtual void loadFromFile(MemoryFile & file) = 0;
};

#endif // __INTERFACES__LOADABLE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
