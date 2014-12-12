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
#ifndef __FT__LIBRARY_H__
#define __FT__LIBRARY_H__

/* Includes */

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftlcdfil.h>
#include <assert.h>
#include <stdio.h>


/* Classes */

class FTFont;

/** C++ interface for the FTLibrary library. */
class FTLibrary
{
public:
	/** Constructor. */
	inline FTLibrary()
	{
		FT_Error err = FT_Init_FreeType(&m_Handle);
		if (err != 0)
			reportError("FT_Init_FreeType", err);
		else
		{
		#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING
			err = FT_Library_SetLcdFilter(m_Handle, FT_LCD_FILTER_DEFAULT);
			if (err != 0)
				reportError("FT_Library_SetLcdFilter", err);
		#endif
		}
	}

	/** Destructor. */
	inline ~FTLibrary()
	{
		FT_Done_FreeType(m_Handle);
	}

	/**
	 * Loads the font.
	 * @param file Name of the font file.
	 */
	FTFont * loadFont(const char * file);

	/**
	 * Translates FreeType error code into string representation.
	 * @param code Error code to translate.
	 */
	static const char * getErrorText(const FT_Error code);

	/**
	 * Reports FreeType error into the log.
	 * @param func Name of the function where error has occured.
	 * @param code Error code.
	 */
	static inline void reportError(const char * func, const FT_Error code)
	{
		fprintf(stderr, "Error: %s failed: %s\n", func, getErrorText(code)); 
	}

private:
	FT_Library m_Handle;		/**< FreeType library object. */
	FT_StreamRec stream;		/**< FreeType stream. */
};

#endif // __FT__LIBRARY_H__
