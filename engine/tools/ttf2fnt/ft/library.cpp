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
#include "library.h"
#include "font.h"

/* FTLibrary methods */

//
// Loads the font
//
FTFont * FTLibrary::loadFont(const char * file)
{
	FT_Face face;

	assert(file);

	FT_Error err = FT_New_Face(m_Handle, file, 0, &face);
	if (err != 0)
	{
		reportError("FT_New_Face", err);
		return NULL;
	}

	return new FTFont(face);
}

//
// Translates FreeType error code into string representation.
//
const char * FTLibrary::getErrorText(const FT_Error code)
{
	#undef __FTERRORS_H__
	#define FT_ERRORDEF( e, v, s )  { e, s },
	#define FT_ERROR_START_LIST     {
	#define FT_ERROR_END_LIST       { 0, 0 } };

	struct errInfo
	{
		FT_Error		errorCode;
		const char *	errorMessage;
	};
	
	static const errInfo ftErrors[] =
	#include FT_ERRORS_H

	for (const errInfo * p = ftErrors; p->errorMessage != NULL; p++)
	{
		if (p->errorCode == code)
			return p->errorMessage;
	}

	return "Unknown freetype error.";
}
