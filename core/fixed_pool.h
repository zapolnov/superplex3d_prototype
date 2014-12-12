/*
 * Copyright (C) 2009 PolarGears, Inc.
 * Portions Copyright (C) John W. Ratcliff, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) John W. Ratcliff, 2001"
 */
#ifndef __CORE__FIXED_POOL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__FIXED_POOL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/exception.h>
#include <tbb/spin_mutex.h>
#include <QtCore/QString>


/* Classes */

/** Template class to manage a fixed pool of items for extremely fast allocation and deallocation. */
template <class T> class FixedPool
{
public:
	/**
	 * Constructor.
	 * @param maxItems Maximum number of items.
	 */
	FixedPool(int maxItems)
		: m_MaxItems(maxItems), m_Current(NULL), m_Head(NULL), m_UsedCount(0), m_FreeCount(maxItems)
	{
		int i;

		m_Data = new Item[m_MaxItems];
		m_Free = m_Data;

		for (i = 0; i < m_MaxItems - 1; i++)
		{
			m_Data[i].next = &m_Data[i + 1];
			m_Data[i].prev = &m_Data[i - 1];
		}

		m_Data[0].prev = NULL;
		m_Data[m_MaxItems - 1].next = NULL;
		m_Data[m_MaxItems - 1].prev = &m_Data[i - 1];
	}

	/** Destructor. */
	~FixedPool()
	{
		delete[] m_Data;
	}

	/** Allocates one node from the pool. */
	T * allocateNode()
	{
		tbb::spin_mutex::scoped_lock lock(m_Mutex);

		if (unlikely(!m_Free))
			throw Exception(QString("Out of space in the pool of %1").arg(typeid(T).name()));

		Item * ret = m_Free;
		Item * temp = m_Head;		// Current head of the list

		m_Free = ret->next;			// New head of free list
		m_Head = ret;        		// New head of list is this free one

		if (temp)
			temp->prev = ret;

		m_Head->next = temp;
		m_Head->prev = 0;

		m_UsedCount++;
		m_FreeCount--;

		return &ret->value;
	}

	/**
	 * Releases the specified node.
	 * @param node Node to release.
	 */
	void release(T * node)
	{
		tbb::spin_mutex::scoped_lock lock(m_Mutex);

		Item * ptr = reinterpret_cast<Item *>(node);
		if (unlikely(ptr->magic != 0xDEADBEEFUL))
			throw Exception(QString("Invalid or corrupt memory block passed as argument into the FixedPool::release()"));

		if (ptr == m_Current)
			m_Current = ptr->next;

		// first patch old linked list.. his previous now points to his next
		Item * prev = ptr->prev;
		if (prev)
		{
			Item * next = ptr->next;
			prev->next = next; // my previous now points to my next
			if (next)
				next->prev = prev;
		}
		else
		{
			Item * next = ptr->next;
			m_Head = next;
			if (m_Head)
				m_Head->prev = NULL;
		}

		Item * temp = m_Free;	// old head of free list.
		m_Free = ptr;			// new head of linked list.

		ptr->prev = NULL;
		ptr->next = temp;

		m_UsedCount--;
		m_FreeCount++;
	}

	/** Returns number of allocated objects. */
	inline int numUsed() const
	{
		return m_UsedCount;
	}

	/** Returns number of free objects. */
	inline int numFree() const
	{
		return m_FreeCount;
	}

private:
	/** Item in the pool. */
	struct Item
	{
		T value;		/**< Stored data. */
		uint32 magic;	/**< Magic value for safety checks. */
		Item * next;	/**< Pointer to the next item. */
		Item * prev;	/**< Pointer to the previous item. */

		/** Constructor. */
		inline Item()
			: magic(0xDEADBEEF), next(NULL), prev(NULL)
		{
		}
	};

	tbb::spin_mutex m_Mutex;	/**< Mutex for synchronization. */
	int m_MaxItems;				/**< Size of the pool. */
	Item * m_Current;			/**< Location of the current iteration. */
	Item * m_Data;				/**< Data storage. */
	Item * m_Head;				/**< Head of the list of used items. */
	Item * m_Free;				/**< Head of the list of free items. */
	int m_UsedCount;			/**< Number of used items. */
	int m_FreeCount;			/**< Number of free items. */
};

#endif // __CORE__FIXED_POOL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
