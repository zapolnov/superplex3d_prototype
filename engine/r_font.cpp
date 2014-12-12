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
#include <engine/render.h>
#include <engine/r_opengl.h>
#include <engine/filesys.h>
#include <map>
#include <GL/glfw.h>
#include <opengl/glext.h>
#include "../tools/ttf2fnt/fntfile.h"


/* Structures */

/** Pair of character codes. */
struct CharPair
{
	uint16 left;		/**< Left character. */
	uint16 right;		/**< Right character. */

	/**
	 * Constructor.
	 * @param l Left character.
	 * @param r Right character.
	 */
	inline CharPair(uint16 l, uint16 r)
		: left(l), right(r)
	{
	}

	/**
	 * Comparison operator.
	 * @param p Pair to compare with.
	 */
	inline bool operator==(const CharPair & p) const
	{
		return left == p.left && right == p.right;
	}

	/**
	 * Comparison operator.
	 * @param p Pair to compare with.
	 */
	inline bool operator<(const CharPair & p) const
	{
		return left < p.left || (left == p.left && right < p.right);
	}
};

/** Font. */
struct FontImpl : public Font
{
	GLuint textureID;								/**< OpenGL ID of the texture. */
	float xScale;									/**< Scale of the X texture coordinate. */
	float yScale;									/**< Scale of the Y texture coordinate. */
	uint height;									/**< Height of the font in pixels. */
	std::map<uint, uint> charToGlyph;				/**< Maps character code to the glyph index. */
	std::map<uint16, FntFileGlyphInfo> glyphs;		/**< Information about glyphs. */
	std::map<CharPair, FntFileKerningInfo> kerning;	/**< Kerning information. */

	/** Constructor. */
	inline FontImpl()
	{
		glGenTextures(1, &textureID);
	}

	/** Destructor. */
	inline ~FontImpl()
	{
		glDeleteTextures(1, &textureID);
	}
};


/* Private functions */

//
// Advances position taking kerning into account
//
static
void R_KerningAdj(const FontImpl * font, const CharPair & pair, int & posX, int & posY)
{
	if (pair.left == 0 && pair.right == 0)
		return;

	std::map<CharPair, FntFileKerningInfo>::const_iterator it = font->kerning.find(pair);
	if (it == font->kerning.end())
		return;

	posX += (int)it->second.x;
	posY += (int)it->second.y;
}


/* Public functions */

//
// Loads the specified font file
//
FontPtr R_LoadFont(const std::string & name) throw(std::exception)
{
	std::string fileData = FS_GetFileContents(name);

	const char * data = fileData.data();
	ulong size = fileData.length();

	// Get pointer to the file header
	if (unlikely(size < sizeof(FntFileHeader)))
		throw std::runtime_error("Invalid font file \"" + name + "\".");
	const FntFileHeader * header = (const FntFileHeader *)data;
	data += sizeof(FntFileHeader);
	size -= sizeof(FntFileHeader);

	// Validate file header
	if (unlikely(header->magic != FntFileMagic))
		throw std::runtime_error("Invalid font file \"" + name + "\".");

	// Get pointer to the character code to glyph mapping table
	if (unlikely(size < sizeof(FntFileCharMapping) * header->nChars))
		throw std::runtime_error("Font file \"" + name + "\" is corrupt.");
	const FntFileCharMapping * mapping = (const FntFileCharMapping *)data;
	data += sizeof(FntFileCharMapping) * header->nChars;
	size -= sizeof(FntFileCharMapping) * header->nChars;

	// Get pointer to the glyph info
	if (unlikely(size < sizeof(FntFileGlyphInfo) * header->nGlyphs))
		throw std::runtime_error("Font file \"" + name + "\" is corrupt.");
	const FntFileGlyphInfo * glyphs = (const FntFileGlyphInfo *)data;
	data += sizeof(FntFileGlyphInfo) * header->nGlyphs;
	size -= sizeof(FntFileGlyphInfo) * header->nGlyphs;

	// Get pointer to the kerning information
	if (unlikely(size < sizeof(FntFileKerningInfo) * header->nKernings))
		throw std::runtime_error("Font file \"" + name + "\" is corrupt.");
	const FntFileKerningInfo * kerning = (const FntFileKerningInfo *)data;
	data += sizeof(FntFileKerningInfo) * header->nKernings;
	size -= sizeof(FntFileKerningInfo) * header->nKernings;

	// Get pointer to the bitmap
	if (unlikely(size != (ulong)(header->bmpWidth * header->bmpHeight)))
	{
		if (size < (ulong)(header->bmpWidth * header->bmpHeight))
			throw std::runtime_error("Font file \"" + name + "\" is corrupt.");
		else
			throw std::runtime_error("Extra data at the end of the font file \"" + name + "\".");
	}
	const char * bitmap = data;

	// Initialize the font structure
	FontPtr font = new FontImpl;
	FontImpl * f = font.as<FontImpl>();
	f->xScale = 1.0f / (float)header->bmpWidth;
	f->yScale = 1.0f / (float)header->bmpHeight;
	f->height = header->fontHeight;
	for (uint i = 0; i < header->nChars; i++)
		f->charToGlyph[mapping[i].code] = mapping[i].glyph;
	for (uint i = 0; i < header->nGlyphs; i++)
		f->glyphs[glyphs[i].id] = glyphs[i];
	for (uint i = 0; i < header->nKernings; i++)
		f->kerning[CharPair(kerning[i].left, kerning[i].right)] = kerning[i];

	// Load bitmap into video memory
	if (ARB_multitexture)
		pglActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, f->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, header->bmpWidth, header->bmpHeight, 0, GL_ALPHA,
		GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (likely(EXT_texture_edge_clamp))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return font;
}

//
// Draws string using the specified font
//
QSize R_DrawString(const FontPtr & font, int posX, int posY, const std::wstring & str, ulong argb)
{
	const FontImpl * f = font.as<FontImpl>();
	QSize size(0, f->height);
	CharPair pair(0, 0);

	float color[4] =
	{
		((argb >> 24) & 0xFF) * (1.0f / 255.0f),
		((argb >> 16) & 0xFF) * (1.0f / 255.0f),
		((argb >>  8) & 0xFF) * (1.0f / 255.0f),
		((argb      ) & 0xFF) * (1.0f / 255.0f)
	};
	glColor4fv(color);

	if (ARB_multitexture)
		pglActiveTexture(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, f->textureID);
	glBegin(GL_QUADS);
	for (std::wstring::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		std::map<uint, uint>::const_iterator jt = f->charToGlyph.find(*it);
		pair.right = (likely(jt != f->charToGlyph.end()) ? jt->second : 0);

		// Adjust for kerning
		int tmpY = posY;
		R_KerningAdj(f, pair, size.width(), tmpY);
		pair.left = pair.right;

		// Find the glyph
		std::map<uint16, FntFileGlyphInfo>::const_iterator kt = f->glyphs.find(pair.right);
		if (unlikely(kt == f->glyphs.end()))
		{
			if (unlikely(pair.right == 0))
				continue;
			kt = f->glyphs.find(0);
			if (unlikely(kt == f->glyphs.end()))
				continue;
		}

		// Calculate on-screen coordinates
		float x1 = (float)(posX + size.width() + kt->second.left);
		float y1 = (float)(tmpY + size.height() - kt->second.top);
		float x2 = x1 + (float)kt->second.width;
		float y2 = y1 + (float)kt->second.height;

		// Calculate texture coordinates
		float tx1 = (float)(kt->second.x) * f->xScale;
		float ty1 = (float)(kt->second.y) * f->yScale;
		float tx2 = (float)(kt->second.x + kt->second.width) * f->xScale;
		float ty2 = (float)(kt->second.y + kt->second.height) * f->yScale;

		// Draw the quad
		glTexCoord2f(tx1, ty1); glVertex2f(x1, y1);
		glTexCoord2f(tx2, ty1); glVertex2f(x2, y1);
		glTexCoord2f(tx2, ty2); glVertex2f(x2, y2);
		glTexCoord2f(tx1, ty2); glVertex2f(x1, y2);

		size.width() += kt->second.advanceX;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	return size;
}

//
// Calculates width and height of the specified text string
//
QSize R_CalcStringMetrics(const FontPtr & font, const std::wstring & str)
{
	const FontImpl * f = font.as<FontImpl>();
	QSize size(0, f->height);
	CharPair pair(0, 0);

	for (std::wstring::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		std::map<uint, uint>::const_iterator jt = f->charToGlyph.find(*it);
		pair.right = (likely(jt != f->charToGlyph.end()) ? jt->second : 0);

		// Adjust for kerning
		int tmpY = size.height();
		R_KerningAdj(f, pair, size.width(), tmpY);
		pair.left = pair.right;

		// Find the glyph
		std::map<uint16, FntFileGlyphInfo>::const_iterator kt = f->glyphs.find(pair.right);
		if (unlikely(kt == f->glyphs.end()))
		{
			if (unlikely(pair.right == 0))
				continue;
			kt = f->glyphs.find(0);
			if (unlikely(kt == f->glyphs.end()))
				continue;
		}

		size.width() += kt->second.advanceX;
	}

	return size;
}
