#ifndef __FNTFILE_H__
#define __FNTFILE_H__

//
// Structure of the FNT file:
//
//	FntHeader
//	CharMapping		(repeats nChars times)
//	GlyphInfo		(repeats nGlyphs times)
//	KerningInfo		(repeats nKernings times)
//	Bitmap
//

/* Includes */

#include <core/common.h>


/* Constants */

static const uint32 FntFileMagic = 0x00544E46;	// 'FNT'


/* Structures */

#pragma pack(push, 1)

/** Header of the FNT file. */
struct FntFileHeader
{
	uint32	magic;			/**< Magic number. */
	uint16	nChars;			/**< Number of characters in the file. */
	uint16	nGlyphs;		/**< Number of glyphs in the file. */
	uint32	nKernings;		/**< Number of kerning records in the file. */
	uint16	bmpWidth;		/**< Width of the bitmap. */
	uint16	bmpHeight;		/**< Height of the bitmap. */
	uint16	fontHeight;		/**< Height of the font in pixels. */
	uint16	reserved;		/**< Reserved for future use. */
};

/** Character mapping in the FNT file. */
struct FntFileCharMapping
{
	uint16	code;			/**< Character code. */
	uint16	glyph;			/**< Glyph index. */
};

/** Glyph information in the FNT file. */
struct FntFileGlyphInfo
{
	uint16	id;				/**< ID of the glyph. */
	uint16	x;				/**< X coordinate of the glyph in the bitmap. */
	uint16	y;				/**< Y coordinate of the glyph in the bitmap. */
	uint16	width;			/**< Width of the glyph. */
	uint16	height;			/**< Height of the glyph. */
	int16	top;			/**< Offset of the top position. */
	int16	left;			/**< Offset of the left position. */
	int16	advanceX;		/**< X offset to the next symbol. */
};

/** Kerning information in the FNT file. */
struct FntFileKerningInfo
{
	uint16	left;			/**< Left character. */
	uint16	right;			/**< Right character. */
	float	x;				/**< X kerning. */
	float	y;				/**< Y kerning. */
};

#pragma pack(pop)

#endif // __FNTFILE_H__
