/* Portions Copyright 2001-2004 Unicode, Inc. */
#ifndef __CORE__UNICODE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__UNICODE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/**
 * @brief Contains functions and types for Unicode support.
 *
 * @b Description.
 *
 * Several funtions are included here, forming a complete set of
 * conversions between the three formats. 
 *
 * Each of these routines takes pointers to input buffers and output
 * buffers.  The input buffers are const.
 *
 * Each routine converts the text between *sourceStart and sourceEnd,
 * putting the result into the buffer between *targetStart and
 * targetEnd.
 *
 * @note The end pointers are <i>after</i> the last item: e.g. 
 * 			*(sourceEnd - 1) is the last item.
 *
 * The return result indicates whether the conversion was successful,
 * and if not, whether the problem was in the source or target buffers.
 * (Only the first encountered problem is indicated.)
 *
 * After the conversion, *sourceStart and *targetStart are both
 * updated to point to the end of last text successfully converted in
 * the respective buffers.
 *
 * @b Input @b parameters:
 *
 * @arg sourceStart - pointer to a pointer to the source buffer.
 *		The contents of this are modified on return so that
 *		it points at the next thing to be converted.
 *
 * @arg targetStart - similarly, pointer to pointer to the target buffer.
 *
 * @arg sourceEnd, targetEnd - respectively pointers to the ends of the
 *		two buffers, for overflow checking only.
 *
 * These conversion functions take a UnicodeConversionFlags argument. When this
 * flag is set to strict, both irregular sequences and isolated surrogates
 * will cause an error.  When the flag is set to lenient, both irregular
 * sequences and isolated surrogates are converted.
 *
 * Whether the flag is strict or lenient, all illegal sequences will cause
 * an error return. This includes sequences such as: \<F4 90 80 80\>, \<C0 80\>,
 * or \<A0\> in UTF-8, and values above 0x10FFFF in UTF-32. Conformant code
 * must check for illegal sequences.
 *
 * When the flag is set to lenient, characters over 0x10FFFF are converted
 * to the replacement character; otherwise (when the flag is set to strict)
 * they constitute an error.
 *
 * @b Output @b parameters:
 *
 * The value "UNI_SOURCE_ILLEGAL" is returned from some routines if the input
 * sequence is malformed.  When "UNI_SOURCE_ILLEGAL" is returned, the source
 * value will point to the illegal value that caused the problem. E.g.,
 * in UTF-8 when a sequence is malformed, it points to the start of the
 * malformed sequence.  
 *
 * Author: Mark E. Davis, 1994.
 *
 */

#include <core/common.h>
#include <string>


/* Types */

typedef uint32 UTF32;		/**< UTF-32 character. */
typedef uint16 UTF16;		/**< UTF-16 character. */
typedef uint8 UTF8;			/**< UTF-8 character. */


/* Constants */

/** Some fundamental constants */
enum UnicodeFundamentalConstants
{
	UNI_REPLACEMENT_CHAR = 0x0000FFFD,	/**< Replacement character for invalid characters. */
	UNI_MAX_BMP = 0x0000FFFF,
	UNI_MAX_UTF16 = 0x0010FFFF,			/**< Maximum valid UTF-16 symbol. */
	UNI_MAX_UTF32 = 0x7FFFFFFF,			/**< Maximum valid UTF-32 symbol. */
	UNI_MAX_LEGAL_UTF32 = 0x0010FFFF,	/**< Maximum legal UTF-32 symbol. */
};

/** Result of the conversion */
enum UnicodeConversionResult
{
	UNI_CONVERSION_OK,					/**< Conversion successful. */
	UNI_SOURCE_EXHAUSTED,				/**< Partial character in source, but hit end. */
	UNI_TARGET_EXHAUSTED,				/**< Insufficient room in target for conversion. */
	UNI_SOURCE_ILLEGAL					/**< Source sequence is illegal/malformed. */
};

/** Conversion flags. */
enum UnicodeConversionFlags
{
	UNI_STRICT = 0,						/**< Strict conversion */
	UNI_LENIENT							/**< Allow irregular sequences and isolated surrogates. */
};


/* Functions */

EXTERN_C_BEGIN

/**
 * Converts UTF-8 string to UTF-16 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF8toUTF16(const UTF8 *& sourceStart, const UTF8 * sourceEnd,
	UTF16 *& targetStart, UTF16 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-16 string to UTF-8 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF16toUTF8(const UTF16 *& sourceStart, const UTF16 * sourceEnd, 
	UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-8 string to UTF-32 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF8toUTF32(const UTF8 *& sourceStart, const UTF8 * sourceEnd, 
	UTF32 *& targetStart, UTF32 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-32 string to UTF-8 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF32toUTF8(const UTF32 *& sourceStart, const UTF32 * sourceEnd, 
	UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-16 string to UTF-32 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF16toUTF32(const UTF16 *& sourceStart, const UTF16 * sourceEnd, 
	UTF32 *& targetStart, UTF32 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-32 string to UTF-16 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF32toUTF16(const UTF32 *& sourceStart, const UTF32*  sourceEnd, 
	UTF16 *& targetStart, UTF16 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-8 string to wchar_t type.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertUTF8toWCHAR(const UTF8 *& sourceStart, const UTF8 * sourceEnd, 
		wchar_t *& targetStart, wchar_t * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts UTF-8 string into wchar_t sequence.
 * @param str Source string.
 */
extern "C++" COREAPI std::wstring convertUTF8toWCHAR(const std::string & str);

/**
 * Converts UTF-8 string into wchar_t sequence.
 * @param data Pointer to the source string.
 * @param length Length of the source string.
 */
extern "C++" COREAPI std::wstring convertUTF8toWCHAR(const char * data, unsigned long length);

/**
 * Converts wchar_t type to UTF-8 string.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceEnd Pointer to the end of input data.
 * @param targetStart Pointer to the start of destination buffer.
 * @param targetEnd Pointer to the end of the destination buffer.
 * @param flags Conversion flags.
 */
COREAPI UnicodeConversionResult convertWCHARtoUTF8(const wchar_t *& sourceStart, const wchar_t * sourceEnd, 
		UTF8 *& targetStart, UTF8 * targetEnd, UnicodeConversionFlags flags);

/**
 * Converts wchar_t sequence into UTF-8 string.
 * @param str Source string.
 */
extern "C++" COREAPI std::string convertWCHARtoUTF8(const std::wstring & str);

/**
 * Converts wchar_t sequence into UTF-8 string.
 * @param data Pointer to the source string.
 * @param length Length of the source string.
 */
extern "C++" COREAPI std::string convertWCHARtoUTF8(const wchar_t * data, unsigned long length);

/**
 * Checks whether specified string is a valid UTF-8 string.
 * @param source Pointer to the input string.
 * @param sourceEnd Pointer to the end of the input string.
 */
COREAPI bool isLegalUTF8Sequence(const UTF8 * source, const UTF8 * sourceEnd);

/**
 * Calculates length of the UTF-8 string in characters.
 * @param sourceStart Pointer to the start of input data.
 * @param sourceSize Length of the input data in bytes.
 * @returns length of the UTF-8 string in characters.
 */
COREAPI int calcUTF8StringLength(const UTF8 * sourceStart, int sourceSize);

/**
 * Calculates size of the UTF-8 string in bytes.
 * @param sourceStart Pointer to the start of input data.
 * @param length Length of the input data in characters.
 * @returns length of the UTF-8 string in bytes.
 */
COREAPI int calcUTF8StringSize(const UTF8 * sourceStart, int length);

/**
 * Calculates size needed for UTF-32 to UTF-8 translation.
 * @param source Pointer to the source string.
 * @param len Length of the source string in characters.
 * @param flags Conversion flags.
 * @returns number of bytes needed to store the converted string.
 */
COREAPI int calcSpaceForUTF32toUTF8(const UTF32 * source, int len, UnicodeConversionFlags flags);

/**
 * Calculates size needed for UTF-16 to UTF-8 translation.
 * @param source Pointer to the source string.
 * @param len Length of the source string in characters.
 * @param flags Conversion flags.
 * @returns number of bytes needed to store the converted string.
 */
COREAPI int calcSpaceForUTF16toUTF8(const UTF16 * source, int len, UnicodeConversionFlags flags);

/**
 * Calculates size needed for wchar_t to UTF-8 translation.
 * @param source Pointer to the source string.
 * @param len Length of the source string in characters.
 * @param flags Conversion flags.
 * @returns number of bytes needed to store the converted string.
 */
COREAPI int calcSpaceForWCHARtoUTF8(const wchar_t * source, int len, UnicodeConversionFlags flags);

/**
 * Translates multibyte sequence in local charset into wchar_t sequence.
 * @param source Source string.
 * @returns Converted string.
 */
extern "C++" COREAPI std::wstring convertLocal8BitToWCHAR(const std::string & source);

/**
 * Translates multibyte sequence in local charset into wchar_t sequence.
 * @param data Pointer to the source string.
 * @param length Length of the source string.
 * @returns Converted string.
 */
COREAPI std::wstring convertLocal8BitToWCHAR(const char * data, unsigned long length);

/**
 * Translates wchar_t sequence into multibyte sequence in local charset.
 * @param source Source string.
 * @returns Converted string.
 */
extern "C++" COREAPI std::string convertWCHARtoLocal8Bit(const std::wstring & source);

/**
 * Translates wchar_t sequence into multibyte sequence in local charset.
 * @param data Pointer to the source string.
 * @param length Length of the source string.
 * @returns Converted string.
 */
COREAPI std::string convertWCHARtoLocal8Bit(const wchar_t * data, unsigned long length);

EXTERN_C_END

#endif // __CORE__UNICODE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
