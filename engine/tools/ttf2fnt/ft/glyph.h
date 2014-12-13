/**
 * Copyright (c) 2009 Nikolay Zapolnov
 * Portions Copyright (c) 2004 Niel Waldren
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must
 *     not claim that you wrote the original software. If you use this
 *     software in a product, an acknowledgment in the product
 *     documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must
 *     not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *     distribution.
 */
#ifndef __FT__GLYPH_H__
#define __FT__GLYPH_H__

/* Includes */

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftlcdfil.h>


/* Classes */

/** C++ interface for the FreeType glyph. */
class FTGlyph
{
public:
	/**
	 * Constructor.
	 * @param glyph Pointer to the glyph.
	 */
	inline FTGlyph(const FT_Glyph glyph)
		: m_Handle(glyph)
	{
	}

	/** Destructor. */
	inline ~FTGlyph()
	{
		if (m_Handle != NULL)
			FT_Done_Glyph(m_Handle);
	}

	/** Returns width of the glyph in pixels. */
	inline int width() const
	{
		return m_Handle ? ((FT_BitmapGlyph)m_Handle)->bitmap.width : 0;
	}

	/** Returns height of the glyph in pixels. */
	inline int height() const
	{
		return m_Handle ? ((FT_BitmapGlyph)m_Handle)->bitmap.rows : 0;
	}

	/** Returns offset of the top position. */
	inline int top() const
	{
		return m_Handle ? ((FT_BitmapGlyph)m_Handle)->top : 0;
	}

	/** Returns offset of the left position. */
	inline int left() const
	{
		return m_Handle ? ((FT_BitmapGlyph)m_Handle)->left : 0;
	}

	/** Returns X offset to the next symbol. */
	inline int advanceX() const
	{
		return m_Handle ? (m_Handle->advance.x >> 16) : 0;
	}

	/** Returns glyph data. */
	inline const unsigned char * data() const
	{
		return m_Handle ? ((FT_BitmapGlyph)m_Handle)->bitmap.buffer : NULL;
	}

	/**
	 * Blits glyph to the specified bitmap.
	 * @param bmp Pointer to the target bitmap.
	 * @param x X coordinate in the target bitmap.
	 * @param y Y coordinate in the target bitmap.
	 * @param w Width of the target bitmap.
	 * @returns width of the glyph.
	 */
	unsigned blitToBitmap(char * bmp, const unsigned x, const unsigned y, const unsigned w);

private:
	FT_Glyph m_Handle;	/**< Handle of the glyph. */
};

#endif // __FT__GLYPH_H__
