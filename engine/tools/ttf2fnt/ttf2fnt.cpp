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
#include "ft/library.h"
#include "ft/font.h"
#include "ft/glyph.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <map>
#include <list>
#include "fntfile.h"
#include "bmpfile.h"

/* Macros */

#define DEFAULT		true


/* Structures */

struct CharacterRange
{
	int				first;
	int				last;
	const char *	name;
	bool			enabled;
};


/* Constants */

static const int defaultTexWidth		= 512;
static const int g_DefaultMaxTexHeight	= 512;
static const int defaultSize			= 8;
static const int defaultDpiX			= 72;
static const int defaultDpiY			= 72;

static const int g_MinTextureWidth	= 32;
static const int g_MaxTextureWidth	= 8192;
static const int g_MaxTextureHeight	= 8192;

static CharacterRange g_Ranges[] =
{
	{ 0x00000000, 0x0000007F, "Basic Latin", DEFAULT },
	{ 0x00000080, 0x000000FF, "Latin-1 Supplement", DEFAULT },
	{ 0x00000100, 0x0000017F, "Latin Extended-A", DEFAULT },
	{ 0x00000180, 0x0000024F, "Latin Extended-B", DEFAULT },
	{ 0x000002B0, 0x000002FF, "Spacing Modifier Letters", false },
	{ 0x00000250, 0x000002AF, "IPA Extensions", false },
	{ 0x00000300, 0x0000036F, "Combining Diacritical Marks", false },
	{ 0x00000370, 0x000003FF, "Greek and Coptic", false },
	{ 0x00000400, 0x000004FF, "Cyrillic", DEFAULT },
	{ 0x00000500, 0x0000052F, "Cyrillic Supplement", DEFAULT },
	{ 0x00000530, 0x0000058F, "Armenian", false },
	{ 0x00000590, 0x000005FF, "Hebrew", false },
	{ 0x00000600, 0x000006FF, "Arabic", false },
	{ 0x00000700, 0x0000074F, "Syriac", false },
	{ 0x00000750, 0x0000077F, "Arabic Supplement", false },
	{ 0x00000780, 0x000007BF, "Thaana", false },
	{ 0x000007C0, 0x000007FF, "NKo", false },
	{ 0x00000900, 0x0000097F, "Devanagari", false },
	{ 0x00000980, 0x000009FF, "Bengali", false },
	{ 0x00000A00, 0x00000A7F, "Gurmukhi", false },
	{ 0x00000A80, 0x00000AFF, "Gujarati", false },
	{ 0x00000B00, 0x00000B7F, "Oriya", false },
	{ 0x00000B80, 0x00000BFF, "Tamil", false },
	{ 0x00000C00, 0x00000C7F, "Telugu", false },
	{ 0x00000C80, 0x00000CFF, "Kannada", false },
	{ 0x00000D00, 0x00000D7F, "Malayalam", false },
	{ 0x00000D80, 0x00000DFF, "Sinhala", false },
	{ 0x00000E00, 0x00000E7F, "Thai", false },
	{ 0x00000E80, 0x00000EFF, "Lao", false },
	{ 0x00000F00, 0x00000FFF, "Tibetan", false },
	{ 0x00001000, 0x0000109F, "Myanmar", false },
	{ 0x000010A0, 0x000010FF, "Georgian", false },
	{ 0x00001100, 0x000011FF, "Hangul Jamo", false },
	{ 0x00001200, 0x0000137F, "Ethiopic", false },
	{ 0x00001380, 0x0000139F, "Ethiopic Supplement", false },
	{ 0x000013A0, 0x000013FF, "Cherokee", false },
	{ 0x00001400, 0x0000167F, "Unified Canadian Aboriginal Syllabics", false },
	{ 0x00001680, 0x0000169F, "Ogham", false },
	{ 0x000016A0, 0x000016FF, "Runic", false },
	{ 0x00001700, 0x0000171F, "Tagalog", false },
	{ 0x00001720, 0x0000173F, "Hanunoo", false },
	{ 0x00001740, 0x0000175F, "Buhid", false },
	{ 0x00001760, 0x0000177F, "Tagbanwa", false },
	{ 0x00001780, 0x000017FF, "Khmer", false },
	{ 0x00001800, 0x000018AF, "Mongolian", false },
	{ 0x00001900, 0x0000194F, "Limbu", false },
	{ 0x00001950, 0x0000197F, "Tai Le", false },
	{ 0x00001980, 0x000019DF, "New Tai Lue", false },
	{ 0x000019E0, 0x000019FF, "Khmer Symbols", false },
	{ 0x00001A00, 0x00001A1F, "Buginese", false },
	{ 0x00001B00, 0x00001B7F, "Balinese", false },
	{ 0x00001B80, 0x00001BBF, "Sundanese", false },
	{ 0x00001C00, 0x00001C4F, "Lepcha", false },
	{ 0x00001C50, 0x00001C7F, "Ol Chiki", false },
	{ 0x00001D00, 0x00001D7F, "Phonetic Extensions", false },
	{ 0x00001D80, 0x00001DBF, "Phonetic Extensions Supplement", false },
	{ 0x00001DC0, 0x00001DFF, "Combining Diacritical Marks Supplement", false },
	{ 0x00001E00, 0x00001EFF, "Latin Extended Additional", DEFAULT },
	{ 0x00001F00, 0x00001FFF, "Greek Extended", false },
	{ 0x00002000, 0x0000206F, "General Punctuation", DEFAULT },
	{ 0x00002070, 0x0000209F, "Superscripts and Subscripts", DEFAULT },
	{ 0x000020A0, 0x000020CF, "Currency Symbols", DEFAULT },
	{ 0x000020D0, 0x000020FF, "Combining Diacritical Marks for Symbols", false },
	{ 0x00002100, 0x0000214F, "Letterlike Symbols", DEFAULT },
	{ 0x00002150, 0x0000218F, "Number Forms", false },
	{ 0x00002190, 0x000021FF, "Arrows", DEFAULT },
	{ 0x00002200, 0x000022FF, "Mathematical Operators", false },
	{ 0x00002300, 0x000023FF, "Miscellaneous Technical", false },
	{ 0x00002400, 0x0000243F, "Control Pictures", false },
	{ 0x00002440, 0x0000245F, "Optical Character Recognition", false },
	{ 0x00002460, 0x000024FF, "Enclosed Alphanumerics", false },
	{ 0x00002500, 0x0000257F, "Box Drawing", false },
	{ 0x00002580, 0x0000259F, "Block Elements", false },
	{ 0x000025A0, 0x000025FF, "Geometric Shapes", DEFAULT },
	{ 0x00002600, 0x000026FF, "Miscellaneous Symbols", DEFAULT },
	{ 0x00002700, 0x000027BF, "Dingbats", false },
	{ 0x000027C0, 0x000027EF, "Miscellaneous Mathematical Symbols-A", false },
	{ 0x000027F0, 0x000027FF, "Supplemental Arrows-A", false },
	{ 0x00002800, 0x000028FF, "Braille Patterns", false },
	{ 0x00002900, 0x0000297F, "Supplemental Arrows-B", false },
	{ 0x00002980, 0x000029FF, "Miscellaneous Mathematical Symbols-B", false },
	{ 0x00002A00, 0x00002AFF, "Supplemental Mathematical Operators", false },
	{ 0x00002B00, 0x00002BFF, "Miscellaneous Symbols and Arrows", false },
	{ 0x00002C00, 0x00002C5F, "Glagolitic", false },
	{ 0x00002C60, 0x00002C7F, "Latin Extended-C", DEFAULT },
	{ 0x00002C80, 0x00002CFF, "Coptic", false },
	{ 0x00002D00, 0x00002D2F, "Georgian Supplement", false },
	{ 0x00002D30, 0x00002D7F, "Tifinagh", false },
	{ 0x00002D80, 0x00002DDF, "Ethiopic Extended", false },
	{ 0x00002DE0, 0x00002DFF, "Cyrillic Extended-A", DEFAULT },
	{ 0x00002E00, 0x00002E7F, "Supplemental Punctuation", false },
	{ 0x00002E80, 0x00002EFF, "CJK Radicals Supplement", false },
	{ 0x00002F00, 0x00002FDF, "Kangxi Radicals", false },
	{ 0x00002FF0, 0x00002FFF, "Ideographic Description Characters", false },
	{ 0x00003000, 0x0000303F, "CJK Symbols and Punctuation", false },
	{ 0x00003040, 0x0000309F, "Hiragana", false },
	{ 0x000030A0, 0x000030FF, "Katakana", false },
	{ 0x00003100, 0x0000312F, "Bopomofo", false },
	{ 0x00003130, 0x0000318F, "Hangul Compatibility Jamo", false },
	{ 0x00003190, 0x0000319F, "Kanbun", false },
	{ 0x000031A0, 0x000031BF, "Bopomofo Extended", false },
	{ 0x000031C0, 0x000031EF, "CJK Strokes", false },
	{ 0x000031F0, 0x000031FF, "Katakana Phonetic Extensions", false },
	{ 0x00003200, 0x000032FF, "Enclosed CJK Letters and Months", false },
	{ 0x00003300, 0x000033FF, "CJK Compatibility", false },
	{ 0x00003400, 0x00004DBF, "CJK Unified Ideographs Extension A", false },
	{ 0x00004DC0, 0x00004DFF, "Yijing Hexagram Symbols", false },
	{ 0x00004E00, 0x00009FFF, "CJK Unified Ideographs", false },
	{ 0x0000A000, 0x0000A48F, "Yi Syllables", false },
	{ 0x0000A490, 0x0000A4CF, "Yi Radicals", false },
	{ 0x0000A500, 0x0000A63F, "Vai", false },
	{ 0x0000A640, 0x0000A69F, "Cyrillic Extended-B", DEFAULT },
	{ 0x0000A700, 0x0000A71F, "Modifier Tone Letters", false },
	{ 0x0000A720, 0x0000A7FF, "Latin Extended-D", DEFAULT },
	{ 0x0000A800, 0x0000A82F, "Syloti Nagri", false },
	{ 0x0000A840, 0x0000A87F, "Phags-pa", false },
	{ 0x0000A880, 0x0000A8DF, "Saurashtra", false },
	{ 0x0000A900, 0x0000A92F, "Kayah Li", false },
	{ 0x0000A930, 0x0000A95F, "Rejang", false },
	{ 0x0000AA00, 0x0000AA5F, "Cham", false },
	{ 0x0000AC00, 0x0000D7AF, "Hangul Syllables", false },
	{ 0x0000D800, 0x0000DB7F, "High Surrogates", false },
	{ 0x0000DB80, 0x0000DBFF, "High Private Use Surrogates", false },
	{ 0x0000DC00, 0x0000DFFF, "Low Surrogates", false },
	{ 0x0000E000, 0x0000F8FF, "Private Use Area", false },
	{ 0x0000F900, 0x0000FAFF, "CJK Compatibility Ideographs", false },
	{ 0x0000FB00, 0x0000FB4F, "Alphabetic Presentation Forms", false },
	{ 0x0000FB50, 0x0000FDFF, "Arabic Presentation Forms-A", false },
	{ 0x0000FE00, 0x0000FE0F, "Variation Selectors", false },
	{ 0x0000FE10, 0x0000FE1F, "Vertical Forms", false },
	{ 0x0000FE20, 0x0000FE2F, "Combining Half Marks", false },
	{ 0x0000FE30, 0x0000FE4F, "CJK Compatibility Forms", false },
	{ 0x0000FE50, 0x0000FE6F, "Small Form Variants", false },
	{ 0x0000FE70, 0x0000FEFF, "Arabic Presentation Forms-B", false },
	{ 0x0000FF00, 0x0000FFEF, "Halfwidth and Fullwidth Forms", false },
	{ 0x0000FFF0, 0x0000FFFF, "Specials", false },
};

static const unsigned g_NumRanges = sizeof(g_Ranges) / sizeof(g_Ranges[0]);


/* Private variables */

static std::map<int, int> g_CharToGlyph;
static std::map<int, FTGlyph *> g_Glyphs;
static std::map<int, std::list<int> > g_GlyphsByHeight;
static std::list<int> g_OutputList;
static std::list<FntFileGlyphInfo> g_GlyphInfo;
static std::list<FntFileKerningInfo> g_KerningInfo;


/* Private funtions */

//
// Shows program usage information
//
static
void usage()
{
	fprintf(stderr,
		"Usage: ttf2fnt [options] input_file output_file\n"
		"\n"
		"Where options are:\n"
		"\n"
		"  --help     Show this help screen\n"
		"  --size N   Specify height of the font in pixels (default: %d)\n"
		"  --dpiX N   Specify the horizontal screen resolution (default: %d)\n"
		"  --dpiY N   Specify the vertical screen resolution (default: %d)\n"
		"  --texW N   Specify width of the texture (default: %d)\n"
		"  --maxH N   Specify maximum height of the texture (default: %d)\n"
		"  --altfmt   If specified, .bmp and .info files will be generated instead of .fnt\n"
		"\n",
		defaultSize, defaultDpiX, defaultDpiY, defaultTexWidth, g_DefaultMaxTexHeight
	);

	exit(1);
}

//
// Fetches glyph that will fit into the specified area and remove it from the list
//
static
int extractGlyph(const int width)
{
	for (std::map<int, std::list<int> >::reverse_iterator it = g_GlyphsByHeight.rbegin();
		it != g_GlyphsByHeight.rend(); ++it)
	{
		std::list<int> & glyphs = it->second;

		for (std::list<int>::iterator jt = glyphs.begin(); jt != glyphs.end(); ++jt)
		{
			const int glyph = *jt;

			const int gw = g_Glyphs[glyph]->width();
			if (gw <= width)
			{
				glyphs.erase(jt);
				if (it->second.empty())
					g_GlyphsByHeight.erase(it->first);
				return glyph;
			}
		}
	}

	return -1;
}

//
// Writes bitmap (.BMP) file
//
void WriteBitmap(const char * data, unsigned w, unsigned h, FILE * f)
{
	const unsigned paletteSize = 256 * 4;
	BmpFileHeader hdr;

	// Write file Header
	hdr.bfType = 19778;	// 'BM'
	hdr.bfSize = w * h + sizeof(BmpFileHeader) + paletteSize;
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = sizeof(BmpFileHeader) + paletteSize;
	hdr.biSize = 40;
	hdr.biWidth = w;
	hdr.biHeight = h;
	hdr.biPlanes = 1;
	hdr.biBitCount = 8;
	hdr.biCompression = 0;
	hdr.biSizeImage = 0;
	hdr.biXPelsPerMeter = 0;
	hdr.biYPelsPerMeter = 0;
	hdr.biClrUsed = 0;
	hdr.biClrImportant = 0;
	fwrite(&hdr, 1, sizeof(BmpFileHeader), f);

	// Palette
	unsigned char * palette = new unsigned char[paletteSize];
	for (unsigned i = 0; i < 256; i++)
	{
		palette[i * 4 + 0] = (unsigned char)i;
		palette[i * 4 + 1] = (unsigned char)i;
		palette[i * 4 + 2] = (unsigned char)i;
		palette[i * 4 + 3] = 0;
	}
	fwrite(palette, 1, paletteSize, f);
	delete[] palette;

	// Bitmap
	for (unsigned i = 0; i < h; i++)
		fwrite(&data[(h - i - 1) * w], 1, w, f);
}

//
// Writes the information file
//
void WriteInfoFile(FILE * f)
{
	// Write glyphs info
	fprintf(f, "[glyphs]\n");
	fprintf(f, "; format: index = x,y,width,height,top,left,advanceX\n");
	for (std::list<FntFileGlyphInfo>::iterator it = g_GlyphInfo.begin(); it != g_GlyphInfo.end(); ++it)
	{
		fprintf(f, "%d=%u,%u,%u,%u,%d,%d,%d\n", it->id, it->x, it->y, it->width, it->height,
			it->top, it->left, it->advanceX);
	}
	fprintf(f, "\n");

	// Write kerning info
	fprintf(f, "[kerning]\n");
	fprintf(f, "; format: left:right = x:y\n");
	for (std::list<FntFileKerningInfo>::iterator it = g_KerningInfo.begin(); it != g_KerningInfo.end(); ++it)
		fprintf(f, "%d:%d=%.2f:%.2f\n", it->left, it->right, it->x, it->y);
	fprintf(f, "\n");
}

//
// Writes the FNT file
//
void WriteFontFile(const char * data, unsigned w, unsigned h, int fontSize, FILE * f)
{
	FntFileHeader header;

	// Write header
	header.magic = FntFileMagic;
	header.nChars = (unsigned short)g_CharToGlyph.size();
	header.nGlyphs = (unsigned short)g_GlyphInfo.size();
	header.nKernings = g_KerningInfo.size();
	header.bmpWidth = (unsigned short)w;
	header.bmpHeight = (unsigned short)h;
	header.fontHeight = (unsigned short)fontSize;
	header.reserved = 0;
	fwrite(&header, 1, sizeof(header), f);

	// Write characters -> glyphs mapping
	FntFileCharMapping * mapping = new FntFileCharMapping[header.nChars];
	unsigned i = 0;
	for (std::map<int, int>::iterator it = g_CharToGlyph.begin(); it != g_CharToGlyph.end(); ++it, ++i)
	{
		mapping[i].code = (unsigned short)it->first;
		mapping[i].glyph = (unsigned short)it->second;
	}
	fwrite(mapping, 1, sizeof(FntFileCharMapping) * header.nChars, f);
	delete[] mapping;

	// Write glyphs information
	FntFileGlyphInfo * glyph = new FntFileGlyphInfo[header.nGlyphs];
	i = 0;
	for (std::list<FntFileGlyphInfo>::iterator it = g_GlyphInfo.begin(); it != g_GlyphInfo.end(); ++it, ++i)
		glyph[i] = *it;
	fwrite(glyph, 1, sizeof(FntFileGlyphInfo) * header.nGlyphs, f);
	delete[] glyph;

	// Write kerning information
	FntFileKerningInfo * kerning = new FntFileKerningInfo[header.nKernings];
	i = 0;
	for (std::list<FntFileKerningInfo>::iterator it = g_KerningInfo.begin(); it != g_KerningInfo.end(); ++it, ++i)
		kerning[i] = *it;
	fwrite(kerning, 1, sizeof(FntFileKerningInfo) * header.nKernings, f);
	delete[] kerning;

	// Write the bitmap
	fwrite(data, 1, w * h, f);
}


/* Public functions */

//
// Program entry point
//
int main(int argc, char ** argv)
{
	int size = defaultSize, dpiX = defaultDpiX, dpiY = defaultDpiY, texWidth = defaultTexWidth,
		maxTexHeight = g_DefaultMaxTexHeight;
	const char * inFile = NULL, * outFile = NULL;
	bool isAltFormat = false;

	// Parse the command-line
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			const char * option = &argv[i][2];

			if (!strcmp(option, "help"))
				usage();
			else if (!strcmp(option, "size"))
			{
				if (i == argc - 1)
					usage();
				else
					size = atoi(argv[++i]);
			}
			else if (!strcmp(option, "dpiX"))
			{
				if (i == argc - 1)
					usage();
				else
					dpiX = atoi(argv[++i]);
			}
			else if (!strcmp(option, "dpiY"))
			{
				if (i == argc - 1)
					usage();
				else
					dpiY = atoi(argv[++i]);
			}
			else if (!strcmp(option, "texW"))
			{
				if (i == argc - 1)
					usage();
				else
					texWidth = atoi(argv[++i]);
			}
			else if (!strcmp(option, "maxH"))
			{
				if (i == argc - 1)
					usage();
				else
					maxTexHeight = atoi(argv[++i]);
			}
			else if (!strcmp(option, "altfmt"))
				isAltFormat = true;
			else
			{
				fprintf(stderr, "Error: Unknown option: '%s'", argv[i]);
				usage();
			}
		}
		else if (!inFile)
			inFile = argv[i];
		else if (!outFile)
			outFile = argv[i];
		else
			usage();
	}

	// Check that all required parameters are present
	if (!inFile || !outFile)
		usage();

	// Check command-line parameters for validity
	if (!strcmp(inFile, outFile))
	{
		fprintf(stderr, "Error: input and output file must not be the same!\n");
		return 1;
	}
	if (texWidth < g_MinTextureWidth)
	{
		fprintf(stderr, "Error: texture width must be at least %d!\n", g_MinTextureWidth);
		return 1;
	}
	if (texWidth > g_MaxTextureWidth)
	{
		fprintf(stderr, "Error: texture width must be less than or equal to %d!\n", g_MaxTextureWidth);
		return 1;
	}
	if ((texWidth & -texWidth) != texWidth)
	{
		fprintf(stderr, "Error: texture width must be power of two!\n");
		return 1;
	}
	if (maxTexHeight > g_MaxTextureHeight)
	{
		fprintf(stderr, "Error: maximum texture height must be less than or equal to %d!\n", g_MaxTextureHeight);
		return 1;
	}

	// Read the input file
	FTLibrary * library = new FTLibrary;
	FTFont * font = library->loadFont(inFile);
	if (!font)
		return 1;
	font->setSize(size, dpiX, dpiY);

	// Walk through the ranges and load glyphs for enabled ranges
	for (unsigned i = 0; i < g_NumRanges; i++)
	{
		if (!g_Ranges[i].enabled)
			continue;

		for (int j = g_Ranges[i].first; j <= g_Ranges[i].last; j++)
		{
			// Translate charCode -> glyphIndex
			const int glyph = (int)font->getGlyphIndex((wchar_t)j);

			// Load glyph if it is has not been already loaded
			std::map<int, FTGlyph *>::iterator it = g_Glyphs.find(glyph);
			if (it == g_Glyphs.end())
			{
				FTGlyph * g = font->getGlyph(glyph);
				if (!g)
				{
					fprintf(stderr, "Unable to find glyph %d (character 0x%08X), ignored.\n",
						glyph, (unsigned)j);
					continue;
				}

				g_Glyphs[glyph] = g;
			}

			g_CharToGlyph[j] = glyph;
		}
	}
	printf("%s: loaded %d glyphs for %d characters\n", inFile, (int)g_Glyphs.size(), (int)g_CharToGlyph.size());

	// Separate glyphs by height
	for (std::map<int, FTGlyph *>::iterator it = g_Glyphs.begin(); it != g_Glyphs.end(); ++it)
		g_GlyphsByHeight[it->second->height()].push_back(it->first);

	// Build the output list
	unsigned width = 0, height = 0, maxWidth = 0, maxHeight = 0;
	while (!g_GlyphsByHeight.empty())
	{
		// Do we have to move to the next row?
		if ((int)width >= texWidth)
		{
		nextrow:
			g_OutputList.push_back(-1);				// Next line indicator
			g_OutputList.push_back((int)maxHeight);	// Height of the line

			if (width > maxWidth)
				maxWidth = width;
			width = 0;
			height += maxHeight + 1;
			maxHeight = 0;
		}

		// Find a glyph that can fit in the remaining space
		int glyph = extractGlyph((int)texWidth - (int)width);
		if (glyph == -1)
		{
			if (width == 0)
			{
				fprintf(stderr, "Error: Texture width of %d pixels is too small.\n", texWidth);
				return 1;
			}
			goto nextrow;
		}

		// Get glyph parameters
		const int glyphHeight = g_Glyphs[glyph]->height();
		width += g_Glyphs[glyph]->width();
		if ((int)width < texWidth)	// If we're not at the end of the texture, then add 1-pixel padding
			width++;

		// Update height of the row
		if (glyphHeight > (int)maxHeight)
			maxHeight = (unsigned)glyphHeight;

		// Store the glyph
		g_OutputList.push_back(glyph);
	}

	// Final calculation of texture width and height
	if (width > maxWidth)
		maxWidth = width;
	height += maxHeight;

	// Round height to the next power of two
	unsigned hh;
	for (hh = 1; height > hh; hh <<= 1)
		;
	height = hh;
	if ((int)height > maxTexHeight)
	{
		fprintf(stderr, "Error: Texture is too high (%d pixels). Try increasing texture width.\n",
			height);
		return 1;
	}

	// Allocate memory for the bitmap
	char * outbuf = new char[maxWidth * height];
	memset(outbuf, 0, maxWidth * height);

	// Now generate the bitmap
	unsigned x = 0, y = 0;
	for (std::list<int>::iterator it = g_OutputList.begin(); it != g_OutputList.end(); ++it)
	{
		if (*it == -1)	// Next row indicator
		{
			++it;
			y += *it + 1;
			x = 0;
		}
		else
		{
			assert(*it >= 0 && *it < 65536);

			FTGlyph * glyph = g_Glyphs[*it];
			FntFileGlyphInfo info;

			// Store glyph info
			info.id = (unsigned short)*it;
			info.x = (unsigned short)x;
			info.y = (unsigned short)y;
			info.width = (unsigned short)glyph->width();
			info.height = (unsigned short)glyph->height();
			info.top = (short)glyph->top();
			info.left = (short)glyph->left();
			info.advanceX = (short)glyph->advanceX();
			g_GlyphInfo.push_back(info);

			// Collect kerning info
			for (std::map<int, FTGlyph *>::iterator jt = g_Glyphs.begin(); jt != g_Glyphs.end(); ++jt)
			{
				FT_Vector kV;
				if (font->getKerning(*it, jt->first, kV) && (kV.x != 0 || kV.y != 0))
				{
					FntFileKerningInfo info;

					info.left = (unsigned short)*it;
					info.right = (unsigned short)jt->first;
					info.x = kV.x * 1.0f / 64.0f;
					info.y = kV.y * 1.0f / 64.0f;

					g_KerningInfo.push_back(info);
				}
			}

			// Paint glyph on the bitmap
			x += glyph->blitToBitmap(outbuf, x, y, maxWidth);

			// If we're not at the end of the texture, then add 1-pixel padding
			if (x < maxWidth)
				x++;
		}
	}

	// Write output file
	FILE * o = fopen(outFile, "wb");
	if (!o)
	{
		fprintf(stderr, "Unable to create output file: %s\n", strerror(errno));
		return 1;
	}
	if (!isAltFormat)
		WriteFontFile(outbuf, maxWidth, height, size, o);
	else
	{
		// Write the bitmap
		WriteBitmap(outbuf, maxWidth, height, o);
		fclose(o);

		// Create information file name
		char * buf = new char [65536];
		strcpy(buf, outFile);
		strcat(buf, ".info");

		// Write information file
		o = fopen(buf, "wb");
		if (!o)
		{
			fprintf(stderr, "Unable to create file %s: %s\n", buf, strerror(errno));
			return 1;
		}
		WriteInfoFile(o);

		delete[] buf;
	}
	fclose(o);

	delete outbuf;
	delete font;
	delete library;

	return 0;
}
