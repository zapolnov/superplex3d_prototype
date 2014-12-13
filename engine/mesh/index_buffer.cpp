#include <core/exception.h>
#include <core/trolltech.h>
#include <mesh/index_buffer.h>
#include <stdlib.h>

/* IndexBuffer methods */

//
// Constructor
//
IndexBuffer::IndexBuffer()
	: m_Data(NULL), m_NumIndices(0), m_AllocatedIndices(0)
{
}

//
// Destructor
//
IndexBuffer::~IndexBuffer()
{
	if (m_Data)
		free(m_Data);
}

//
// Resizes the buffer
//
void IndexBuffer::resize(uint nIndices)
{
	if (nIndices > m_AllocatedIndices)
		reserve(nIndices);

	m_NumIndices = nIndices;
}

//
// Reallocates the internal storage without resizing the data
//
void IndexBuffer::reserve(uint nIndices)
{
	if (nIndices <= m_AllocatedIndices)
		return;

	byte * ptr = (byte *)realloc(m_Data, nIndices * 2);
	if (unlikely(!ptr))
		throw Exception(QObject::tr("Out of memory."));

	m_AllocatedIndices = nIndices;
	m_Data = ptr;
}

//
// Adds index into the buffer
//
uint IndexBuffer::addIndex(uint16 idx)
{
	uint index = m_NumIndices;

	resize(m_NumIndices + 1);
	setIndex(index, idx);

	return index;
}

//
// Sets the specified index
//
void IndexBuffer::setIndex(uint idx, uint16 val)
{
	Q_ASSERT(idx < m_NumIndices);
	*((uint16 *)(m_Data + idx * 2)) = val;
}
