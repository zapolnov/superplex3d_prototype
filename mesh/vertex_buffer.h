#ifndef __MESH__VERTEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__VERTEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <mesh/vertex_format.h>
#include <string.h>


/* Classes */

/** Vertex buffer. */
class VertexBuffer
{
public:
	/** Constructor. */
	MESHAPI VertexBuffer();

	/**
	 * Constructor.
	 * @param format Vertex format.
	 */
	MESHAPI VertexBuffer(const VertexFormat & format);

	/** Destructor. */
	MESHAPI ~VertexBuffer();

	/**
	 * Sets vertex format to use.
	 * This function clears the vertex buffer.
	 */
	MESHAPI void setVertexFormat(const VertexFormat & format);

	/** Returns number of vertices in the buffer. */
	inline uint count() const
	{
		return m_NumVertices;
	}

	/** Returns size of a single vertex. */
	inline uint vertexSize() const
	{
		return m_VertexSize;
	}

	/** Returns size of the buffer in bytes. */
	inline uint sizeInBytes() const
	{
		return m_NumVertices * m_VertexSize;
	}

	/**
	 * Resizes the buffer.
	 * @param nVertices Number of vertices to store.
	 */
	MESHAPI void resize(uint nVertices);

	/**
	 * Reallocates the internal storage without resizing the data.
	 * @param nVertices Number of vertices to preallocate.
	 */
	MESHAPI void reserve(uint nVertices);

	/**
	 * Adds vertex into the buffer.
	 * @param ptr Pointer to the vertex data.
	 * @returns index of the vertex.
	 */
	MESHAPI uint addVertex(const void * ptr);

	/**
	 * Sets the specified vertex.
	 * @param idx Index of the vertex.
	 * @param ptr Pointer to the vertex data.
	 */
	inline void setVertex(uint idx, const void * ptr)
	{
		Q_ASSERT(idx < m_NumVertices);
		memcpy(m_Data + idx * m_VertexSize, ptr, m_VertexSize);
	}

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
	 * Returns pointer to the specified vertex data.
	 * @param idx Index of the vertex.
	 */
	inline const void * operator[](uint idx) const
	{
		Q_ASSERT(idx < m_NumVertices);
		return m_Data + idx * m_VertexSize;
	}

	/**
	 * Returns pointer to the specified vertex data.
	 * @param idx Index of the vertex.
	 */
	inline void * operator[](uint idx)
	{
		Q_ASSERT(idx < m_NumVertices);
		return m_Data + idx * m_VertexSize;
	}

	/** Returns vertex format for the data. */
	inline const VertexFormat & format() const
	{
		return m_Format;
	}

private:
	VertexFormat m_Format;		/**< Vertex format. */
	uint m_VertexSize;			/**< Size of a single vertex. */
	byte * m_Data;				/**< Pointer to the data. */
	uint m_NumVertices;			/**< Number of vertices in the buffer. */
	uint m_AllocatedVertices;	/**< Number of vertices that can fit into the allocated buffer. */
};

#endif // __MESH__VERTEX_BUFFER_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
