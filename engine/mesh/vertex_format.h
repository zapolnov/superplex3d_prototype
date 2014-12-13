#ifndef __MESH__VERTEX_FORMAT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__VERTEX_FORMAT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

//
// Attribute order:
//	Position
//	Normal
//	Texture coordinates
//	Color
//	Secondary color
//	Secondary texture coordinates
//	Tangent vector
//

/* Includes */

#include <core/common.h>


/* Constants */

/** Maximum number of attributes a vertex may have. */
static const uint g_MaxVertexAttributes = 16;

/** Attribute semantics. */
enum AttributeSemantic
{
	AttributePosition = 0,					/**< Position. */
	AttributeNormal,						/**< Normal vector. */
	AttributeTexCoord,						/**< Primary texture coordinates. */
	AttributeColor,							/**< Color. */
	AttributeSecondaryColor,				/**< Secondary color. */
	AttributeSecondaryTexCoord,				/**< Secondary texture coordinates. */
	AttributeTangent,						/**< Tangent space vector. */
};

/** Attribute format. */
enum AttributeFormat
{
	FormatUnused = 0,						/**< Attribute is unused. */
	FormatFloat,							/**< Attribute consists of single floating-point number. */
	FormatFloat2,							/**< Attribute consists of 2 floating-point numbers. */
	FormatFloat3,							/**< Attribute consists of 3 floating-point numbers. */
	FormatFloat4,							/**< Attribute consists of 4 floating-point numbers. */
};


/* Classes */

/** Vertex format definition. */
class VertexFormat
{
public:
	/** Default constructor. */
	MESHAPI VertexFormat();

	/** Clears the vertex format definition. */
	MESHAPI void clear();

	/**
	 * Sets format of the specified attribute.
	 * @param attr Attribute to set format of.
	 * @param format Format of the attribute.
	 */
	inline void setAttributeFormat(AttributeSemantic attr, AttributeFormat format)
	{
		Q_ASSERT((int)attr >= 0 && (uint)attr < g_MaxVertexAttributes);
		m_Attributes[attr].format = format;
		m_OffsetsDirty = true;
	}

	/**
	 * Returns true if specified attribute is present in the vertex format.
	 * @param attr Attribute to check for.
	 */
	inline bool isAttributePresent(AttributeSemantic attr) const
	{
		Q_ASSERT((int)attr >= 0 && (uint)attr < g_MaxVertexAttributes);
		return m_Attributes[attr].format != FormatUnused;
	}

	/**
	 * Returns format of the specified attribute.
	 * @param attr Semantic of the attribute.
	 */
	inline AttributeFormat attributeFormat(AttributeSemantic attr) const
	{
		Q_ASSERT((int)attr >= 0 && (uint)attr < g_MaxVertexAttributes);
		return m_Attributes[attr].format;
	}

	/**
	 * Returns offset of the specified attribute.
	 * @param attr Semantic of the attribute.
	 */
	inline uint attributeOffset(AttributeSemantic attr) const
	{
		if (unlikely(m_OffsetsDirty))
			const_cast<VertexFormat *>(this)->calcOffsets();

		Q_ASSERT((int)attr >= 0 && (uint)attr < g_MaxVertexAttributes);
		Q_ASSERT(m_Attributes[attr].format != FormatUnused);
		return m_Attributes[attr].offset;
	}

	/** Returns size of the vertex format. */
	MESHAPI uint size() const;

	/** Returns number of bytes required to store vertex format definition in the stream. */
	inline uint storedSize() const
	{
		return sizeof(AttributeFormat) * g_MaxVertexAttributes;
	}

	/**
	 * Loads vertex format definition from the external stream.
	 * @param data Pointer to the data.
	 */
	MESHAPI void load(const void * data);

	/**
	 * Stores vertex format definition in the external stream.
	 * @param data Pointer to the buffer.
	 */
	MESHAPI void store(void * data) const;

	/**
	 * Returns size of the specified attribute format.
	 * @param format Format to return size of.
	 */
	MESHAPI static uint attributeFormatSize(AttributeFormat format);

private:
	/** Attribute information. */
	struct Attribute
	{
		AttributeFormat format;								/**< Attribute format. */
		uint offset;										/**< Attribute offset. */
	};

	Attribute m_Attributes[g_MaxVertexAttributes];			/**< Attributes information. */
	bool m_OffsetsDirty;									/**< If true, offsets need to be recalculated. */

	/** Calculates offsets of the attributes. */
	MESHAPI void calcOffsets();
};

#endif // __MESH__VERTEX_FORMAT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
