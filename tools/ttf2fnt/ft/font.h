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
#ifndef __FT__FONT_H__
#define __FT__FONT_H__

/* Includes */

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftlcdfil.h>
#include <assert.h>


/* Classes */

class FTGlyph;

/** C++ interface for the FreeType font. */
class FTFont
{
public:
	/**
	 * Constructor.
	 * @param face Pointer to the freetype font face.
	 */
	inline FTFont(FT_Face face)
		: m_Handle(face)
	{
		assert(face);
	}

	/** Destructor. */
	inline ~FTFont()
	{
		FT_Done_Face(m_Handle);
	}

	/**
	 * Sets size of the font.
	 * @param size Size of the font in points.
	 * @param dpi_x Horizontal DPI.
	 * @param dpi_y Vertical DPI.
	 */
	void setSize(const int size, const int dpi_x, const int dpi_y);

	/**
	 * Returns index of the glyph for the specified character.
	 * @param ch Character code.
	 */
	FT_UInt getGlyphIndex(const wchar_t ch);

	/**
	 * Loads glyph with the specified index.
	 * @param idx Glyph index.
	 */
	FTGlyph * getGlyph(const int idx);

	/** Returns true if it is a scalable (vector) font. */
	inline bool isScalable() const
	{
		return FT_IS_SCALABLE(m_Handle);
	}

	/** Returns true if it is a SFNT-based font. */
	inline bool isSFNT() const
	{
		return FT_IS_SFNT(m_Handle) != 0;
	}

	/** Returns true if this font has kerning. */
	inline bool hasKerning() const
	{
		return FT_HAS_KERNING(m_Handle) != 0;
	}

	/** Returns the ascent of the font. */
	inline float ascent() const
	{
		return (float)(m_Handle->size->metrics.y_ppem) - descent();
	}

	/** Returns the descent of the font. */
	inline float descent() const
	{
    	return (float)(m_Handle->size->metrics.descender) * (-1.0f / 64.0f);
	}

	/** Returns line height of the font. */
	inline float lineHeight() const
	{
	    return (ascent() + descent());
	}

	/** Returns number of glyphs in the font. */
	inline FT_Long getNumGlyphs() const
	{
		return m_Handle->num_glyphs;
	}

	/**
	 * Returns kerning adjustment for the right character depending on the left character.
     * @param left First character.
     * @param right Second character.
     * @param kerningVector Destination variable.
     * @returns true on success, false on failure.
     */
	inline bool getKerning(int left, int right, FT_Vector & kerningVector)
	{
		return FT_Get_Kerning(m_Handle, (FT_UInt)left, (FT_UInt)right, FT_KERNING_DEFAULT, &kerningVector) == 0;
	}	

private:
	FT_Face m_Handle;	/**< Face handle. */
};

#endif // __FT__FONT_H__
