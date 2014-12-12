#ifndef __CORE__REF_COUNTED_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__REF_COUNTED_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <tbb/atomic.h>


/* Classes */

/** Base class for all reference-counted objects. */
class GCC_PUBLIC RefCounted
{
public:
	/** Constructor. */
	inline RefCounted()
	{
		m_RefCount = 0;
	}

	/** Destructor. */
	COREAPI virtual ~RefCounted();

	/** Increments reference counter of the object. */
	inline void addRef() const
	{
		int old_value = m_RefCount.fetch_and_increment();

		Q_ASSERT_MSG(old_value >= 0, "Underflow of RefCountedObject reference counter");
		(void)old_value;
	}

	/**
	 * Decrements reference counter of the object.
	 * @returns true if this was the last instance of the object and it has been freed, false otherwise.
	 */
	inline bool release() const
	{
		int old_value = m_RefCount.fetch_and_decrement();
		Q_ASSERT_MSG(old_value > 0, "Underflow of RefCountedObject reference counter");

		if (old_value == 1)
		{
			delete this;
			return true;
		}

		return false;
	}

private:
	mutable tbb::atomic<int> m_RefCount;	/**< Reference counter */
};

#endif // __CORE__REF_COUNTED_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
