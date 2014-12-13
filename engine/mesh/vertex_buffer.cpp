#include <core/exception.h>
#include <core/trolltech.h>
#include <mesh/vertex_buffer.h>
#include <stdlib.h>

/* VertexBuffer methods */

//
// Constructor
//
VertexBuffer::VertexBuffer()
	: m_VertexSize(16), m_Data(NULL), m_NumVertices(0), m_AllocatedVertices(0)
{
}

//
// Constructor
//
VertexBuffer::VertexBuffer(const VertexFormat & format)
	: m_Format(format), m_VertexSize(format.size()), m_Data(NULL), m_NumVertices(0), m_AllocatedVertices(0)
{
}

//
// Destructor
//
VertexBuffer::~VertexBuffer()
{
	if (m_Data)
		free(m_Data);
}

//
// Sets vertex format to use
//
void VertexBuffer::setVertexFormat(const VertexFormat & format)
{
	uint bufferSize = m_AllocatedVertices * m_VertexSize;

	m_Format = format;
	m_VertexSize = format.size();

	m_NumVertices = 0;
	m_AllocatedVertices = bufferSize / m_VertexSize;
}

//
// Resizes the buffer
//
void VertexBuffer::resize(uint nVertices)
{
	if (nVertices > m_AllocatedVertices)
		reserve(nVertices);

	m_NumVertices = nVertices;
}

//
// Reallocates the internal storage without resizing the data
//
void VertexBuffer::reserve(uint nVertices)
{
	if (nVertices <= m_AllocatedVertices)
		return;

	byte * ptr = (byte *)realloc(m_Data, nVertices * m_VertexSize);
	if (unlikely(!ptr))
		throw Exception(QObject::tr("Out of memory."));

	m_AllocatedVertices = nVertices;
	m_Data = ptr;
}

//
// Adds vertex into the buffer
//
uint VertexBuffer::addVertex(const void * ptr)
{
	uint index = m_NumVertices;

	resize(m_NumVertices + 1);
	setVertex(index, ptr);

	return index;
}
