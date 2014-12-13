#include <mesh/vertex_format.h>
#include <string.h>

/* Private variables */

/** Sizes of attribute formats. */
static uint g_FormatSize[] =
{
	0,
	1 * sizeof(float),
	2 * sizeof(float),
	3 * sizeof(float),
	4 * sizeof(float)
};


/* VertexFormat methods */

//
// Default constructor
//
VertexFormat::VertexFormat()
	: m_OffsetsDirty(false)
{
	memset(m_Attributes, 0, sizeof(m_Attributes));

	Q_ASSERT(g_FormatSize[FormatUnused] == 0);
	Q_ASSERT(g_FormatSize[FormatFloat] == 1 * sizeof(float));
	Q_ASSERT(g_FormatSize[FormatFloat2] == 2 * sizeof(float));
	Q_ASSERT(g_FormatSize[FormatFloat3] == 3 * sizeof(float));
	Q_ASSERT(g_FormatSize[FormatFloat4] == 4 * sizeof(float));
}

//
// Clears the vertex format definition
//
void VertexFormat::clear()
{
	memset(m_Attributes, 0, sizeof(m_Attributes));
	m_OffsetsDirty = false;
}

//
// Returns size of the vertex format
//
uint VertexFormat::size() const
{
	uint size = 0;

	for (uint i = 0; i < g_MaxVertexAttributes; i++)
	{
		Q_ASSERT((int)m_Attributes[i].format >= 0 && m_Attributes[i].format < (int)(sizeof(g_FormatSize) / sizeof(g_FormatSize[0])));
		size += g_FormatSize[m_Attributes[i].format];
	}

	return size;
}

//
// Loads vertex format definition from the external stream
//
void VertexFormat::load(const void * data)
{
	const AttributeFormat * format = (const AttributeFormat *)data;

	for (uint i = 0; i < g_MaxVertexAttributes; i++)
		m_Attributes[i].format = *format++;

	m_OffsetsDirty = true;
}

//
// Stores vertex format definition in the external stream
//
void VertexFormat::store(void * data) const
{
	AttributeFormat * format = (AttributeFormat *)data;

	for (uint i = 0; i < g_MaxVertexAttributes; i++)
		*format++ = m_Attributes[i].format;
}

//
// Returns size of the specified attribute format
//
uint VertexFormat::attributeFormatSize(AttributeFormat format)
{
	Q_ASSERT((int)format >= 0 && format < (int)(sizeof(g_FormatSize) / sizeof(g_FormatSize[0])));
	return g_FormatSize[format];
}

//
// Calculates offsets of the attributes
//
void VertexFormat::calcOffsets()
{
	uint size = 0;

	for (uint i = 0; i < g_MaxVertexAttributes; i++)
	{
		m_Attributes[i].offset = size;

		Q_ASSERT((int)m_Attributes[i].format >= 0 && m_Attributes[i].format < (int)(sizeof(g_FormatSize) / sizeof(g_FormatSize[0])));
		size += g_FormatSize[m_Attributes[i].format];
	}

	m_OffsetsDirty = false;
}
