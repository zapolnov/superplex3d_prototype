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
#include "glyph.h"
#include "font.h"
#include "library.h"

/* FTFont methods */

//
// Sets size of the font.
//
void FTFont::setSize(const int size, const int dpi_x, const int dpi_y)
{
	FT_Error err = FT_Set_Char_Size(m_Handle, size << 6, size << 6, (FT_UInt)dpi_x, (FT_UInt)dpi_y);
	if (err != 0)
		FTLibrary::reportError("FT_Set_Char_Size", err);
}

//
// Returns index of the glyph for the specified character
//
FT_UInt FTFont::getGlyphIndex(const wchar_t ch)
{
	return FT_Get_Char_Index(m_Handle, ch);
}

//
// Loads glyph with the specified index
//
FTGlyph * FTFont::getGlyph(const int idx)
{
	FT_Error err = FT_Load_Char(m_Handle, (FT_ULong)idx, FT_LOAD_RENDER);
	if (err != 0)
	{
		FTLibrary::reportError("FT_Load_Char", err);
		return NULL;
	}

	err = FT_Load_Glyph(m_Handle, (FT_ULong)idx, FT_LOAD_NO_HINTING);
    if (err != 0)
    {
		FTLibrary::reportError("FT_Load_Glyph", err);
		return NULL;
    }

	FT_Glyph glyph;
	err = FT_Get_Glyph(m_Handle->glyph, &glyph);
	if (err != 0)
	{
		FTLibrary::reportError("FT_Get_Glyph", err);
		return NULL;
	}

	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	if (err != 0)
	{
		FTLibrary::reportError("FT_Glyph_To_Bitmap", err);
		return NULL;
	}

	if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
	{
		fprintf(stderr, "Error: Glyph is not a bitmap.\n");
		return NULL;
	}

   	return new FTGlyph(glyph);
}
