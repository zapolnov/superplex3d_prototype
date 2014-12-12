#include <core/unicode.h>

#ifdef WIN32
 #include <windows.h>
#endif


/* Private variables */

static const int	halfShift			= 10;
static const UTF32	halfBase 			= 0x0010000UL;
static const UTF32	halfMask 			= 0x3FFUL;
static const UTF32	UNI_SUR_HIGH_START  = 0xD800;
static const UTF32	UNI_SUR_HIGH_END	= 0xDBFF;
static const UTF32	UNI_SUR_LOW_START	= 0xDC00;
static const UTF32	UNI_SUR_LOW_END		= 0xDFFF;

// Index into the table below with the first byte of a UTF-8 sequence to
// get the number of trailing bytes that are supposed to follow it.
// Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
// left as-is for anyone who may want to do such conversion, which was
// allowed in earlier algorithms.
static const char g_TrailingBytesForUTF8[256] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

// Magic values subtracted from a buffer value during UTF8 conversion.
// This table contains as many values as there might be trailing bytes
// in a UTF-8 sequence.
static const UTF32 g_OffsetsFromUTF8[6] =
{
	0x00000000UL, 0x00003080UL, 0x000E2080UL, 
	0x03C82080UL, 0xFA082080UL, 0x82082080UL 
};

// Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
// into the first byte, depending on how many bytes follow.  There are
// as many entries in this table as there are UTF-8 sequence types.
// (I.e., one byte sequence, two byte... etc.). Remember that sequences
// for *legal* UTF-8 will be 4 or fewer bytes total.
static const UTF8 g_FirstByteMark[7] =
{
	0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};


/* Private functions */

//
// Utility routine to tell whether a sequence of bytes is legal UTF-8.
// This must be called with the length pre-determined by the first byte.
// If not calling this from ConvertUTF8to*, then the length can be set by:
// length = g_TrailingBytesForUTF8[*source] + 1;
// and the sequence is illegal right away if there aren't that many bytes
// available.
// If presented with a length > 4, this returns false.
// The Unicode definition of UTF-8 goes up to 4-byte sequences.
//
static inline bool isLegalUTF8(const UTF8 * source, int length)
{
    const UTF8 * srcptr = source + length;
	UTF8 a;

	switch (length)
	{
	default:
		return false;

	// Everything else falls through when "true"...
    case 4:
		if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
			return false;
    case 3:
		if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
			return false;
    case 2:
		if ((a = (*--srcptr)) > 0xBF)
			return false;
		switch (*source)
		{
    	// no fall-through in this inner switch
		case 0xE0:
			if (a < 0xA0)
				return false;
			break;
	    case 0xED:
			if (a > 0x9F)
				return false;
			break;
	    case 0xF0:
			if (a < 0x90)
				return false;
			break;
    	case 0xF4:
			if (a > 0x8F)
				return false;
			break;
		default:
			if (a < 0x80)
				return false;
		}
    case 1:
		if (*source >= 0x80 && *source < 0xC2)
			return false;
    }

	return (*source <= 0xF4);
}


/* Public functions */

//
// Converts UTF-32 string to UTF-16 string
//
UnicodeConversionResult convertUTF32toUTF16(const UTF32 *& sourceStart, const UTF32 * sourceEnd, 
	UTF16 *& targetStart, UTF16 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;

    while (sourceStart < sourceEnd)
	{
		UTF32 ch;

		if (unlikely(targetStart >= targetEnd))
		{
	    	result = UNI_TARGET_EXHAUSTED;
			break;
		}

		ch = *sourceStart++;
		if (ch <= UNI_MAX_BMP)
		{
			// Target is a character <= 0xFFFF
		    if (ch < UNI_SUR_HIGH_START || ch > UNI_SUR_LOW_END)
				*targetStart++ = (UTF16)ch;	// normal case
			else
			{
			    // UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values
				if (likely(flags != UNI_STRICT))
				    *targetStart++ = UNI_REPLACEMENT_CHAR;
				else
				{
					--sourceStart;	 				// return to the illegal value itself
					result = UNI_SOURCE_ILLEGAL;
					break;
				}
			}
		}
		else if (ch > UNI_MAX_LEGAL_UTF32)
		{
			if (unlikely(flags == UNI_STRICT))
				result = UNI_SOURCE_ILLEGAL;
			else
				*targetStart++ = UNI_REPLACEMENT_CHAR;
		}
		else
		{
			// target is a character in range 0xFFFF - 0x10FFFF.
	    	if (unlikely(targetStart + 1 >= targetEnd))
			{
				--sourceStart;						// Back up source pointer!
				result = UNI_TARGET_EXHAUSTED;
				break;
		    }
		    ch -= halfBase;
		    *targetStart++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
	    	*targetStart++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
		}
    }

    return result;
}

//
// Converts UTF-16 string to UTF-32 string
//
UnicodeConversionResult convertUTF16toUTF32(const UTF16 *& sourceStart, const UTF16* sourceEnd, 
	UTF32 *& targetStart, UTF32 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;
	UTF32 ch, ch2;

	while (sourceStart < sourceEnd)
	{
		const UTF16 * oldSource = sourceStart; // In case we have to back up because of target overflow.
		ch = *sourceStart++;

		// If we have a surrogate pair, convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
	    	// If the 16 bits following the high surrogate are in the source buffer...
			if (likely(sourceStart < sourceEnd))
			{
				ch2 = *sourceStart;
				// If it's a low surrogate, convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) +
						 (ch2 - UNI_SUR_LOW_START) + halfBase;
					++sourceStart;
				}
				else if (unlikely(flags == UNI_STRICT))
				{
					// It's an unpaired high surrogate - return to the illegal value itself
					--sourceStart;
					result = UNI_SOURCE_ILLEGAL;
					break;
				}
			}
			else
			{
				// We don't have the 16 bits following the high surrogate - return to the high surrogate
				--sourceStart;
				result = UNI_SOURCE_EXHAUSTED;
				break;
			}
		}
		else if (flags == UNI_STRICT)
		{
	    	// UTF-16 surrogate values are illegal in UTF-32
			if (unlikely(ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END))
			{
				// Return to the illegal value itself
				--sourceStart;
				result = UNI_SOURCE_ILLEGAL;
				break;
	    	}
		}
		
		if (unlikely(targetStart >= targetEnd))
		{
		    sourceStart = oldSource; 		// Back up source pointer!
			result = UNI_TARGET_EXHAUSTED;
			break;
		}

		*targetStart++ = ch;
	}

    return result;
}

//
// Converts UTF-16 string to UTF-8 string
//
UnicodeConversionResult convertUTF16toUTF8(const UTF16 *& sourceStart, const UTF16 * sourceEnd, 
	UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;

	while (sourceStart < sourceEnd)
	{
		const UTF32 byteMask = 0xBF, byteMark = 0x80;
		const UTF16 * oldSource = sourceStart;			// In case we have to back up because of target overflow.
		unsigned short bytesToWrite = 0;
		UTF32 ch;

		ch = *sourceStart++;

		// If we have a surrogate pair, convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			// If the 16 bits following the high surrogate are in the source buffer...
			if (likely(sourceStart < sourceEnd))
			{
				UTF32 ch2 = *sourceStart;

				// If it's a low surrogate, convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) +
						 (ch2 - UNI_SUR_LOW_START) + halfBase;
					++sourceStart;
				}
				else if (unlikely(flags == UNI_STRICT))
				{
					// It's an unpaired high surrogate - return to the illegal value itself
					--sourceStart;
					result = UNI_SOURCE_ILLEGAL;
					break;
				}
			}
			else
			{
				// We don't have the 16 bits following the high surrogate - return to the high surrogate.
				--sourceStart;
				result = UNI_SOURCE_EXHAUSTED;
				break;
			}
		}
		else if (flags == UNI_STRICT)
		{
	    	// UTF-16 surrogate values are illegal in UTF-32
			if (unlikely(ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END))
			{
				// return to the illegal value itself
				--sourceStart;
				result = UNI_SOURCE_ILLEGAL;
				break;
			}
		}
	
		// Figure out how many bytes the result will require
		if (ch < (UTF32)0x80)
			bytesToWrite = 1;
		else if (ch < (UTF32)0x800)
			bytesToWrite = 2;
		else if (ch < (UTF32)0x10000)
			bytesToWrite = 3;
		else if (ch < (UTF32)0x110000)
			bytesToWrite = 4;
		else
		{
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}

		targetStart += bytesToWrite;
		if (unlikely(targetStart > targetEnd))
		{
	    	sourceStart = oldSource;			// Back up source pointer!
			targetStart -= bytesToWrite;
			result = UNI_TARGET_EXHAUSTED;
			break;
		}

		switch (bytesToWrite)
		{
		// note: everything falls through.
	    case 4:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 3:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 2:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 1:
			*--targetStart = (UTF8)(ch | g_FirstByteMark[bytesToWrite]);
		}

		targetStart += bytesToWrite;
	}

    return result;
}

//
// Converts UTF-8 string to UTF-16 string
//
UnicodeConversionResult convertUTF8toUTF16(const UTF8 *& sourceStart, const UTF8 * sourceEnd, 
	UTF16 *& targetStart, UTF16 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;

	while (sourceStart < sourceEnd)
	{
		unsigned short extraBytesToRead = g_TrailingBytesForUTF8[*sourceStart];
		UTF32 ch = 0;

		if (unlikely(sourceStart + extraBytesToRead >= sourceEnd))
		{
			result = UNI_SOURCE_EXHAUSTED;
			break;
		}

		// Do this check whether lenient or strict
		if (!isLegalUTF8(sourceStart, extraBytesToRead + 1))
		{
			result = UNI_SOURCE_ILLEGAL;
			break;
		}

		// The cases all fall through.
		switch (extraBytesToRead)
		{
	    case 5:
			ch += *sourceStart++;
			ch <<= 6;				// remember, illegal UTF-8
	    case 4:
			ch += *sourceStart++;
			ch <<= 6;				// remember, illegal UTF-8
	    case 3:
			ch += *sourceStart++;
			ch <<= 6;
	    case 2:
			ch += *sourceStart++;
			ch <<= 6;
	    case 1:
			ch += *sourceStart++;
			ch <<= 6;
	    case 0:
			ch += *sourceStart++;
		}
		ch -= g_OffsetsFromUTF8[extraBytesToRead];

		if (unlikely(targetStart >= targetEnd))
		{
	    	sourceStart -= (extraBytesToRead + 1);	 // Back up source pointer!
			result = UNI_TARGET_EXHAUSTED;
			break;
		}

		if (ch <= UNI_MAX_BMP)
		{
			// Target is a character <= 0xFFFF
			if (ch < UNI_SUR_HIGH_START || ch > UNI_SUR_LOW_END)
				*targetStart++ = (UTF16)ch;					// normal case
			else
			{
			    // UTF-16 surrogate values are illegal in UTF-32
				if (likely(flags != UNI_STRICT))
				    *targetStart++ = UNI_REPLACEMENT_CHAR;
				else
				{
				    sourceStart -= (extraBytesToRead + 1);	// return to the illegal value itself
				    result = UNI_SOURCE_ILLEGAL;
				    break;
				}
			}
		}
		else if (ch > UNI_MAX_UTF16)
		{
	    	if (likely(flags != UNI_STRICT))
				*targetStart++ = UNI_REPLACEMENT_CHAR;
			else
			{
				result = UNI_SOURCE_ILLEGAL;
				sourceStart -= (extraBytesToRead + 1);		// return to the start
				break;
			}
		}
		else
		{
	    	// target is a character in range 0xFFFF - 0x10FFFF.
			if (unlikely(targetStart + 1 >= targetEnd))
			{
				sourceStart -= (extraBytesToRead + 1);		// Back up source pointer!
				result = UNI_TARGET_EXHAUSTED;
				break;
			}
			ch -= halfBase;
			*targetStart++ = (UTF16)((ch >> halfShift) + UNI_SUR_HIGH_START);
			*targetStart++ = (UTF16)((ch & halfMask) + UNI_SUR_LOW_START);
		}
	}

    return result;
}

//
// Converts UTF-32 to UTF-8
//
UnicodeConversionResult convertUTF32toUTF8(const UTF32 *& sourceStart, const UTF32 * sourceEnd, 
	UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;

	while (sourceStart < sourceEnd)
	{
		const UTF32 byteMask = 0xBF, byteMark = 0x80;
		unsigned short bytesToWrite = 0;
		UTF32 ch;

		ch = *sourceStart++;
		if (flags == UNI_STRICT)
		{
	    	// UTF-16 surrogate values are illegal in UTF-32
		    if (unlikely(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END))
			{
				--sourceStart;		 // return to the illegal value itself
				result = UNI_SOURCE_ILLEGAL;
				break;
			}
		}

		// Figure out how many bytes the result will require. Turn any
		// illegally large UTF32 things (> Plane 17) into replacement chars.
		if (ch < (UTF32)0x80)
			bytesToWrite = 1;
		else if (ch < (UTF32)0x800)
			bytesToWrite = 2;
		else if (ch < (UTF32)0x10000)
			bytesToWrite = 3;
		else if (ch <= UNI_MAX_LEGAL_UTF32)
			bytesToWrite = 4;
		else
		{
			bytesToWrite = 3;
		    ch = UNI_REPLACEMENT_CHAR;
			result = UNI_SOURCE_ILLEGAL;
		}

		targetStart += bytesToWrite;
		if (unlikely(targetStart > targetEnd))
		{
			--sourceStart;			// Back up source pointer!
			targetStart -= bytesToWrite;
			result = UNI_TARGET_EXHAUSTED;
			break;
		}

		switch (bytesToWrite)
		{
		// note: everything falls through.
	    case 4:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 3:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 2:
			*--targetStart = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
	    case 1:
			*--targetStart = (UTF8)(ch | g_FirstByteMark[bytesToWrite]);
		}

		targetStart += bytesToWrite;
	}

    return result;
}

//
// Converts UTF-8 string to UTF-32 string
//
UnicodeConversionResult convertUTF8toUTF32(const UTF8 *& sourceStart, const UTF8 * sourceEnd, 
	UTF32 *& targetStart, UTF32 * targetEnd, UnicodeConversionFlags flags)
{
	UnicodeConversionResult result = UNI_CONVERSION_OK;

	while (sourceStart < sourceEnd)
	{
		unsigned short extraBytesToRead = g_TrailingBytesForUTF8[*sourceStart];
		UTF32 ch = 0;

		if (unlikely(sourceStart + extraBytesToRead >= sourceEnd))
		{
			result = UNI_SOURCE_EXHAUSTED;
			break;
		}

		// Do this check whether lenient or strict
		if (!isLegalUTF8(sourceStart, extraBytesToRead + 1))
		{
		    result = UNI_SOURCE_ILLEGAL;
		    break;
		}

		// The cases all fall through.
		switch (extraBytesToRead)
		{
	    case 5:
			ch += *sourceStart++;
			ch <<= 6;
	    case 4:
			ch += *sourceStart++;
			ch <<= 6;
	    case 3:
			ch += *sourceStart++;
			ch <<= 6;
	    case 2:
			ch += *sourceStart++;
			ch <<= 6;
	    case 1:
			ch += *sourceStart++;
			ch <<= 6;
	    case 0:
			ch += *sourceStart++;
		}
		ch -= g_OffsetsFromUTF8[extraBytesToRead];

		if (unlikely(targetStart >= targetEnd))
		{
		    sourceStart -= (extraBytesToRead + 1); 		// Back up the source pointer!
		    result = UNI_TARGET_EXHAUSTED;
			break;
		}

		if (likely(ch <= UNI_MAX_LEGAL_UTF32))
		{
	    	// UTF-16 surrogate values are illegal in UTF-32, and anything over
			// Plane 17 (> 0x10FFFF) is illegal.
			if (ch < UNI_SUR_HIGH_START || ch > UNI_SUR_LOW_END)
				*targetStart++ = ch;
			else
			{
				if (likely(flags != UNI_STRICT))
				    *targetStart++ = UNI_REPLACEMENT_CHAR;
				else
				{
					sourceStart -= (extraBytesToRead + 1); 	// return to the illegal value itself
					result = UNI_SOURCE_ILLEGAL;
					break;
				}
			}
		}
		else
		{
			// i.e., ch > UNI_MAX_LEGAL_UTF32
		    result = UNI_SOURCE_ILLEGAL;
		    *targetStart++ = UNI_REPLACEMENT_CHAR;
		}
	}

	return result;
}

//
// Converts UTF-8 string to wchar_t type
//
UnicodeConversionResult convertUTF8toWCHAR(const UTF8 *& sourceStart, const UTF8 * sourceEnd,
	wchar_t *& targetStart, wchar_t * targetEnd, UnicodeConversionFlags flags)
{
	Q_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);
	if (sizeof(wchar_t) == 2)
		return convertUTF8toUTF16(sourceStart, sourceEnd, (UTF16 *&)targetStart, (UTF16 *)targetEnd, flags);
	else
		return convertUTF8toUTF32(sourceStart, sourceEnd, (UTF32 *&)targetStart, (UTF32 *)targetEnd, flags);
}

//
// Converts UTF-8 string into wchar_t sequence
//
std::wstring convertUTF8toWCHAR(const std::string & str)
{
	return convertUTF8toWCHAR(str.data(), str.length());
}

//
// Converts UTF-8 string into wchar_t sequence.
//
std::wstring convertUTF8toWCHAR(const char * data, unsigned long length)
{
	Q_ASSERT(sizeof(UTF8) == 1);

	int size = calcUTF8StringLength((const UTF8 *)data, length);
	std::wstring result(size, L' ');

	const UTF8 * sourceStart = (const UTF8 *)data;
	const UTF8 * sourceEnd = (const UTF8 *)data + length;
	wchar_t * targetStart = (wchar_t *)result.data();
	wchar_t * targetEnd = targetStart + size;

	convertUTF8toWCHAR(sourceStart, sourceEnd, targetStart, targetEnd, UNI_LENIENT);
	result.resize((targetStart - (wchar_t *)result.data()));

	return result;
}

//
// Converts wchar_t type to UTF-8 string
//
UnicodeConversionResult convertWCHARtoUTF8(const wchar_t *& sourceStart, const wchar_t * sourceEnd,
	UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags)
{
	Q_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);
	if (sizeof(wchar_t) == 2)
		return convertUTF16toUTF8((const UTF16 *&)sourceStart, (UTF16 *)sourceEnd, targetStart, targetEnd, flags);
	else
		return convertUTF32toUTF8((const UTF32 *&)sourceStart, (UTF32 *)sourceEnd, targetStart, targetEnd, flags);
}

//
// Converts wchar_t sequence into UTF-8 string
//
std::string convertWCHARtoUTF8(const std::wstring & str)
{
	return convertWCHARtoUTF8(str.data(), str.length());
}

/**
 * Converts wchar_t sequence into UTF-8 string.
 * @param data Pointer to the source string.
 * @param length Length of the source string.
 */
std::string convertWCHARtoUTF8(const wchar_t * data, unsigned long length)
{
	Q_ASSERT(sizeof(UTF8) == 1);

	int size = calcSpaceForWCHARtoUTF8(data, length, UNI_LENIENT);
	std::string result(size, ' ');

	const wchar_t * sourceStart = data;
	const wchar_t * sourceEnd = data + length;
	UTF8 * targetStart = (UTF8 *)result.data();
	UTF8 * targetEnd = targetStart + size;

	convertWCHARtoUTF8(sourceStart, sourceEnd, targetStart, targetEnd, UNI_LENIENT);
	result.resize((targetStart - (UTF8 *)result.data()));

	return result;
}

//
// Checks whether a UTF-8 sequence is legal or not.
//
bool isLegalUTF8Sequence(const UTF8 * source, const UTF8 * sourceEnd)
{
	int length = g_TrailingBytesForUTF8[*source] + 1;
	if (unlikely(source + length > sourceEnd))
		return false;
    return isLegalUTF8(source, length);
}

//
// Calculates length of the UTF-8 string.
//
int calcUTF8StringLength(const UTF8 * sourceStart, int sourceSize)
{
	int length = 0;

	const UTF8 * sourceEnd = sourceStart + sourceSize;
	while (sourceStart < sourceEnd)
	{
		size_t charLen = g_TrailingBytesForUTF8[*sourceStart] + 1;
		sourceStart += charLen;
		if (likely(sourceStart <= sourceEnd))
			length++;
	}

	return length;
}

//
// Calculates size of the UTF-8 string
//
int calcUTF8StringSize(const UTF8 * sourceStart, int length)
{
	const UTF8 * ptr = sourceStart;

	while (length-- > 0)
		ptr += g_TrailingBytesForUTF8[*ptr] + 1;

	return ptr - sourceStart;
}

//
// Calculates number of bytes needed for conversion to UTF-8
//
int calcSpaceForUTF32toUTF8(const UTF32 * source, int len, UnicodeConversionFlags flags)
{
	int result = 0;

	Q_ASSERT(source);
	while (len >= 0)
	{
		UTF32 ch;

		ch = *source++;
		--len;

		if (flags == UNI_STRICT)
		{
	    	// UTF-16 surrogate values are illegal in UTF-32
		    if (unlikely(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END))
				break;
		}

		// Figure out how many bytes the result will require. Turn any
		// illegally large UTF32 things (> Plane 17) into replacement chars.
		if (ch < (UTF32)0x80)
			result++;
		else if (ch < (UTF32)0x800)
			result += 2;
		else if (ch < (UTF32)0x10000)
			result += 3;
		else if (ch <= UNI_MAX_LEGAL_UTF32)
			result += 4;
		else
			result += 3;
	}

    return result;
	
}

//
// Calculates number of bytes needed for conversion to UTF-8
//
int calcSpaceForUTF16toUTF8(const UTF16 * source, int len, UnicodeConversionFlags flags)
{
	int result = 0;

	Q_ASSERT(source);
	while (len >= 0)
	{
		UTF32 ch;

		ch = *source++;
		--len;

		// If we have a surrogate pair, convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			// If the 16 bits following the high surrogate are in the source buffer...
			if (unlikely(!*source))
				break;
			else
			{
				if (unlikely(len <= 0))
					break;

				UTF32 ch2 = *source;

				// If it's a low surrogate, convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) +
						 (ch2 - UNI_SUR_LOW_START) + halfBase;
					++source;
					--len;
				}
				else if (unlikely(flags == UNI_STRICT))
					break;
			}
		}
		else if (flags == UNI_STRICT)
		{
	    	// UTF-16 surrogate values are illegal in UTF-32
			if (unlikely(ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END))
				break;
		}
	
		// Figure out how many bytes the result will require
		if (ch < (UTF32)0x80)
			result++;
		else if (ch < (UTF32)0x800)
			result += 2;
		else if (ch < (UTF32)0x10000)
			result += 3;
		else if (ch < (UTF32)0x110000)
			result += 4;
		else
			result += 3;
	}

    return result;
}

//
// Calculates size needed for wchar_t to UTF-8 translation
//
int calcSpaceForWCHARtoUTF8(const wchar_t * source, int len, UnicodeConversionFlags flags)
{
	Q_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);
	if (sizeof(wchar_t) == 2)
		return calcSpaceForUTF16toUTF8((const UTF16 *)source, len, flags);
	else
		return calcSpaceForUTF32toUTF8((const UTF32 *)source, len, flags);
}

//
// Translates multibyte sequence in local charset into wchar_t sequence
//
std::wstring convertLocal8BitToWCHAR(const std::string & source)
{
	return convertLocal8BitToWCHAR(source.data(), source.length());
}

//
// Translates multibyte sequence in local charset into wchar_t sequence
//
std::wstring convertLocal8BitToWCHAR(const char * data, unsigned long length)
{
	Q_ASSERT(data);

#ifdef WIN32
	int result = MultiByteToWideChar(CP_ACP, 0, data, length, NULL, 0);
	if (result >= 0)
	{
		std::wstring value(result, L' ');

		result = MultiByteToWideChar(CP_ACP, 0, data, length, (wchar *)value.data(), result);
		if (result >= 0)
			return value;
	}

	return std::wstring();
#else
	return convertUTF8toWCHAR(data, length);		// FIXME?
#endif
}

//
// Translates wchar_t sequence into multibyte sequence in local charset
//
std::string convertWCHARtoLocal8Bit(const std::wstring & source)
{
	return convertWCHARtoLocal8Bit(source.data(), source.length());
}

//
// Translates wchar_t sequence into multibyte sequence in local charset
//
std::string convertWCHARtoLocal8Bit(const wchar_t * data, unsigned long length)
{
#ifdef WIN32
	int result = WideCharToMultiByte(CP_ACP, 0, data, length, NULL, 0, NULL, NULL);
	if (result >= 0)
	{
		std::string value(result, ' ');

		result = WideCharToMultiByte(CP_ACP, 0, data, length, (char *)value.data(), result, NULL, NULL);
		if (result >= 0)
			return value;
	}
#else
	return convertWCHARtoUTF8(data, length);		// FIXME?
#endif

	return std::string();
}
