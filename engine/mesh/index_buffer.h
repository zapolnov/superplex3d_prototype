#ifndef __MESH__INDEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__INDEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Classes */

/** Index buffer. */
class IndexBuffer
{
public:
	/** Constructor. */
	MESHAPI IndexBuffer();

	/** Destructor. */
	MESHAPI ~IndexBuffer();

	/** Returns number of indices in the buffer. */
	inline uint count() const
	{
		return m_NumIndices;
	}

	/** Returns size of a single index. */
	inline uint indexSize() const
	{
		return 2;
	}

	/** Returns size of the buffer in bytes. */
	inline uint sizeInBytes() const
	{
		return m_NumIndices * 2;
	}

	/**
	 * Resizes the buffer.
	 * @param nIndices Number of indices to store.
	 */
	MESHAPI void resize(uint nIndices);

	/**
	 * Reallocates the internal storage without resizing the data.
	 * @param nIndices Number of indices to preallocate.
	 */
	MESHAPI void reserve(uint nIndices);

	/**
	 * Adds index into the buffer.
	 * @param idx Index to add.
	 * @returns offset of the index.
	 */
	MESHAPI uint addIndex(const uint16 idx);

	/**
	 * Sets the specified index.
	 * @param idx Offset of the index.
	 * @param val Value of the index.
	 */
	MESHAPI void setIndex(uint idx, const uint16 val);

	/** Returns pointer to the entire buffer. */
	inline const void * constData() const
	{
		Q_ASSERT(m_Data);
		return m_Data;
	}

	/** Returns pointer to the entire buffer. */
	inline const void * data() const
	{
		Q_ASSERT(m_Data);
		return m_Data;
	}

	/** Returns pointer to the entire buffer. */
	inline void * data()
	{
		Q_ASSERT(m_Data);
		return m_Data;
	}

	/**
	 * Returns the specified index data.
	 * @param idx Offset of the index.
	 */
	inline uint16 operator[](uint idx) const
	{
		Q_ASSERT(idx < m_NumIndices);
		return *((uint16 *)(m_Data + idx * 2));
	}

	/**
	 * Returns reference to the specified index data.
	 * @param idx Offset of the index.
	 */
	inline uint16 & operator[](uint idx)
	{
		Q_ASSERT(idx < m_NumIndices);
		return *((uint16 *)(m_Data + idx * 2));
	}

private:
	byte * m_Data;				/**< Pointer to the data. */
	uint m_NumIndices;			/**< Number of indices in the buffer. */
	uint m_AllocatedIndices;	/**< Number of indices that can fit into the allocated buffer. */
};

#endif // __MESH__INDEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
