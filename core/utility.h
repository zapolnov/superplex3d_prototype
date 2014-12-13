#ifndef __CORE__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <cmake_config.h>
#include <core/common.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <core/trolltech.h>


NAMESPACE_INTERNAL_BEGIN

/* Macros */

#define PG_UNICODE(x) (*(x))


/* Constants */

/** Size of the conversion buffer. */
static const int CVTBUFSIZE = 309 + 43;

/** Flag: Pad with zero. */
static const int ZEROPAD = 0x01;
/** Flag: Unsigned/signed long. */
static const int SIGN = 0x02;
/** Flag: Show plus. */
static const int PLUS = 0x04;
/** Flag: Space if plus. */
static const int SPACE = 0x08;
/** Flag: Left justified. */
static const int LEFT = 0x10;
/** Flag: 0x. */
static const int SPECIAL = 0x20;
/** Flag: Use 'ABCDEF' instead of 'abcdef'. */
static const int LARGE = 0x40;


/* Variables */

EXTERN_C_BEGIN

COREAPI extern const char * lowerDigits;
COREAPI extern const char * upperDigits;

COREAPI extern const wchar * wLowerDigits;
COREAPI extern const wchar * wUpperDigits;

EXTERN_C_END


/* Functions */

/**
 * Converts string to integer.
 * @param fmt Pointer to the string.
 * @returns read number.
 */
template <class CHAR> int skipAtoI(const CHAR * & fmt)
{
	int i = 0;

	while (*fmt >= L'0' && *fmt <= L'9')
	{
		i = i * 10 + PG_UNICODE(fmt) - L'0';
		fmt++;
	}

	return i;
}

//
// Converts floating-point number to string
//
template <class CHAR, class FLT> CHAR * convertFloatingPoint(FLT arg, int ndigits, int * decpt,
	int * sign, CHAR * buf, const bool doublePrecision)
{
	long double fi, fj;
	CHAR * p, * p1;
	int r2;

	if (ndigits < 0)
		ndigits = 0;
	if (ndigits >= CVTBUFSIZE - 1)
		ndigits = CVTBUFSIZE - 2;

	r2 = 0;
	*sign = 0;
	p = &buf[0];

	if (arg < 0)
	{
		*sign = 1;
		arg = -arg;
	}
	arg = (FLT)modfl(arg, &fi);
	p1 = &buf[CVTBUFSIZE];

	if (fi != 0)
	{
		p1 = &buf[CVTBUFSIZE];
		while (fi != 0)
		{
			fj = modfl(fi / 10, &fi);
			*--p1 = (CHAR)((int)((fj + .03) * 10) + '0');
			r2++;
		}
		while (p1 < &buf[CVTBUFSIZE])
			*p++ = *p1++;
	}
	else if (arg > 0)
	{
		while ((fj = arg * 10) < 1)
		{
			arg = (FLT)fj;
			r2--;
		}
	}

	p1 = &buf[ndigits];
	if (!doublePrecision)
		p1 += r2;
	*decpt = r2;
	if (p1 < &buf[0])
	{
		buf[0] = 0;
		return buf;
	}

	while (p <= p1 && p < &buf[CVTBUFSIZE])
	{
		arg *= 10;
		arg = (FLT)modfl(arg, &fj);
		*p++ = (CHAR)((int)fj + '0');
	}

	if (p1 >= &buf[CVTBUFSIZE])
	{
		buf[CVTBUFSIZE - 1] = 0;
		return buf;
	}

	p = p1;
	*p1 = (CHAR)(*p1 + 5);
	while (*p1 > '9')
	{
		*p1 = '0';

		if (p1 > buf)
		{
			--p1;
			*p1 = *p1 + 1;
		}
		else
		{
			*p1 = '1';
			(*decpt)++;
			if (!doublePrecision)
			{
				if (p > buf)
					*p = '0';
				p++;
			}
		}
	}

	*p = 0;
	return buf;
}

//
// Converts floating-point number to text
//
template <class CHAR, class FLT, class TWC, TWC toLower(const TWC)>
	void floatingPointToString(const FLT value, CHAR * buffer, CHAR fmt, int precision)
{
	int decpt, sign, exp, pos, capexp = 0, magnitude;
	CHAR * digits = NULL, cvtbuf[CVTBUFSIZE];

	if (fmt == 'G' || fmt == 'E')
	{
		capexp = 1;
		fmt = (CHAR)toLower(fmt);
	}

	if (fmt == 'g')
	{
		digits = convertFloatingPoint<CHAR, FLT>(value, precision, &decpt, &sign, cvtbuf, true);
		magnitude = decpt - 1;
		if (magnitude < -4 || magnitude > precision - 1)
		{
			fmt = 'e';
			precision -= 1;
		}
		else
		{
			fmt = 'f';
			precision -= decpt;
		}
	}

	if (fmt == 'e')
	{
		digits = convertFloatingPoint<CHAR, FLT>(value, precision + 1, &decpt, &sign, cvtbuf, true);

		if (sign)
			*buffer++ = '-';
		*buffer++ = *digits;
        if (precision > 0)
			*buffer++ = '.';
		memcpy(buffer, digits + 1, precision);
        *buffer++ = capexp ? 'E' : 'e';

		if (decpt != 0)
			exp = decpt - 1;
		else
		{
			if (value == 0.0f)
				exp = 0;
			else
				exp = -1;
		}

		if (exp >= 0)
			*buffer++ = '+';
		else
		{
			*buffer++ = '-';
			exp = -exp;
        }

		CHAR ch2 = (CHAR)((exp % 10) + '0');
        exp = exp / 10;
        CHAR ch1 = (CHAR)((exp % 10) + '0');
        exp = exp / 10;
        *buffer++ = (CHAR)((exp % 10) + '0');
        *buffer++ = ch1;
        *buffer++ = ch2;
	}
	else if (fmt == 'f')
	{
		digits = convertFloatingPoint<CHAR, FLT>(value, precision, &decpt, &sign, cvtbuf, false);

		if (sign)
			*buffer++ = '-';

		if (*digits)
        {
			if (decpt <= 0)
			{
				*buffer++ = '0';
				*buffer++ = '.';
				for (pos = 0; pos < -decpt; pos++)
					*buffer++ = '0';
        		while (*digits)
        			*buffer++ = *digits++;
			}
			else
			{
				pos = 0;
				while (*digits)
				{
					if (pos++ == decpt)
						*buffer++ = '.';
					*buffer++ = *digits++;
				}
			}
		}
		else
		{
			*buffer++ = '0';
			if (precision > 0)
			{
				*buffer++ = '.';
				for (pos = 0; pos < precision; pos++)
					*buffer++ = '0';
			}
		}
	}

	*buffer = 0;
}

/**
 * Adds decimal point to the number in the buffer if it is not present yet.
 * @param buffer Pointer to the source/destination buffer.
 */
template <class CHAR, size_t strLen(const CHAR *)> void forceDecimalPoint(CHAR * buffer)
{
	while (*buffer)
 	{
    	if (*buffer == '.')
    		return;
    	if (*buffer == 'e' || *buffer == 'E')
    		break;
		buffer++;
	}

	if (*buffer)
	{
		int n = strLen(buffer);

		while (n > 0) 
		{
      		buffer[n + 1] = buffer[n];
      		n--;
		}

		*buffer = '.';
  	}
  	else
  	{
    	*buffer++ = '.';
    	*buffer = 0;
  	}
}

/**
 * Crop trailing zeros from the floating-point number.
 * @param buffer Pointer to the work buffer.
 */
template <class CHAR> void cropZeros(CHAR * buffer)
{
	CHAR * stop;

	while (*buffer && *buffer != '.')
		buffer++;

	if (*buffer++)
	{
	    while (*buffer && *buffer != 'e' && *buffer != 'E')
	    	buffer++;
		stop = buffer--;
		while (*buffer == '0')
			buffer--;
		if (*buffer == '.')
			buffer--;
   		while ((*++buffer = *stop++))
   			;
	}
}

/**
 * Formats the floating-point number.
 * @param target Reference to the destination array.
 * @param fvalue Number to format.
 * @param fieldWidth Maximum number of character the number can use.
 * @param precision	Required precision.
 * @param fmt Format character (one that is after %).
 * @param flags Conversion flags.
 */
template <class TGT, class CHAR, class FLT, class TWC, TWC toLower(TWC), size_t strLen(const CHAR *)>
	void formatFloatingPoint(TGT & target, FLT fvalue, int fieldWidth,
		int precision, const CHAR fmt, int flags)
{
	CHAR c, sign = 0, tmp[CVTBUFSIZE];
	int n;

	// Left align means no zero padding
	if (flags & LEFT)
		flags &= ~ZEROPAD;

	// Determine padding and sign char
	c = (flags & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (flags & SIGN)
	{
		if (fvalue < 0.0)
		{
			sign = '-';
			fvalue = -fvalue;
			fieldWidth--;
		}
		else if (flags & PLUS)
		{
			sign = '+';
			fieldWidth--;
		}
		else if (flags & SPACE)
		{
			sign = ' ';
			fieldWidth--;
		}
	}

	// Compute the precision value
	if (precision < 0)
		precision = 6;
	else if (precision == 0 && fmt == 'g')
		precision = 1;

	// Convert floating point number to text
	floatingPointToString<CHAR, FLT, TWC, toLower>(fvalue, tmp, fmt, precision);

	// '#' and precision == 0 means force a decimal point
	if ((flags & SPECIAL) && precision == 0)
		forceDecimalPoint<CHAR, strLen>(tmp);

	// 'g' format means crop zero unless '#' given
	if (fmt == 'g' && !(flags & SPECIAL))
		cropZeros(tmp);

	// Output number with alignment and padding
	n = strLen(tmp);
	fieldWidth -= n;

	// Left padding
	if (!(flags & (ZEROPAD | LEFT)))
	{
		while (fieldWidth-- > 0)
			target.append(' ');
	}

	// Put sign
	if (sign != 0)
		target.append(sign);

	// Zero padding
	if (!(flags & LEFT))
	{
		while (fieldWidth-- > 0)
			target.append(c);
	}

	// Number
	target.append(tmp);

	// Right padding
	while (fieldWidth-- > 0)
		target.append(' ');
}

//
// Returns a string equivalent of the specified number
//
template <class CHAR, class TYPE, class TGT> static inline
void convNumber(TGT & arr, const CHAR * digits, TYPE n, const TYPE base = 10)
{
	Q_ASSERT(base >= 2 && base <= 36);

	arr.clear();
	arr.reserve(32);

	#ifdef _MSC_VER
		#pragma warning(disable:4146)	// Unary minus operator applied to unsigned type
	#endif

	// Not easy way.. To eliminate warning on unsigned types
	if (n > 0)
		;
	else if (n == 0)
		;
	else	
	{
		arr.append('-');
		n = -n;
	}		

	#ifdef _MSC_VER
		#pragma warning(default:4146)
	#endif

	do
	{
		const TYPE num = n % base;
		n /= base;

		arr.append(digits[num]);
	}
	while (n != 0);
}

//
// Formats the decimal number
//
template <class TGT, class CHAR, class TYPE> static inline
void formatNumber(TGT & target, const CHAR * ldigits, const CHAR * hdigits, TYPE num,
	int base, int size, int precision, int type)
{
	Q_ASSERT(base >= 2 && base <= 36);

	const CHAR * dig = ldigits;
	CHAR c, sign = 0, tmp[66];
	int i;

	if (type & LARGE)
		dig = hdigits;
	if (type & LEFT)
		type &= ~ZEROPAD;

	c = (type & ZEROPAD) ? '0' : ' ';
	if (type & SIGN)
	{
    	if (num < 0)
	    {
    		sign = '-';
			num = -num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}

	if (type & SPECIAL)
	{
    	if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}

	i = 0;

	if (num == 0)
    	tmp[i++] = '0';
	else
	{
		while (num != 0)
		{
			tmp[i++] = dig[((ulonglong)num) % (unsigned)base];
			num = ((ulonglong)num) / (unsigned)base;
		}
	}

	if (i > precision)
		precision = i;
	size -= precision;

	// Left padding with spaces
	if (!(type & (ZEROPAD | LEFT)))
	{
		while (size-- > 0)
			target.append(' ');
	}

	// Sign
	if (sign != 0)
		target.append(sign);

  	// Special prefix (0 or 0x)
	if (type & SPECIAL)
	{
	    if (base == 8)
	    	target.append('0');
		else if (base == 16)
			target.append("0x");
	}

	// Left padding with zeros
  	if (!(type & LEFT))
  	{
  		while (size-- > 0)
  			target.append(c);
	}

	// Precision
	while (i < precision--)
		target.append('0');

	// Number
	while (i-- > 0)
		target.append(tmp[i]);

	// Right padding
	while (size-- > 0)
		target.append(' ');
}

//
// Counts number of occurences of substring inside the specified string
//
template <class STR> static uint count(const STR & str, const typename STR::value_type * substr,
	const uint len)
{
	const uint srclen = str.size();
	uint result = 0;

	if (len == 0)
		return srclen;
	if (len > srclen)
		return 0;

	const typename STR::const_pointer begin = str.data();
	const typename STR::const_pointer end = begin + srclen - len;
	const typename STR::value_type ch = *substr;

	for (typename STR::const_pointer p = begin; p <= end; ++p)
	{
		if (*p == ch)
		{
			if (!memcmp(p + 1, substr + 1, len - 1))
				result++;
		}
	}

	return result;
}

//
// Replaces all occurences of the substring with another substring
//
template <class STRING, class ITERATOR1, class REPL> void replace(STRING & str,
	ITERATOR1 begin1, ITERATOR1 end1, REPL replacement, const uint aftersz)
{
	const uint stringsz = str.size();
	const uint beforesz = end1 - begin1;

	if (stringsz == 0 || beforesz == 0)
		return;

	for (uint pos = 0; pos < stringsz; )
	{
		typename STRING::iterator it = std::search(str.begin() + pos, str.end(), begin1, end1);
		if (it == str.end())
			break;
		pos = it - str.begin() + aftersz;
		str.replace(it, it + beforesz, replacement);
	}
}

//
// Replaces all occurences of the specified character with the specified substring
//
template <class STRING, class REPL> void replace(STRING & str, const typename STRING::value_type ch, 
	REPL replacement, const uint aftersz)
{
	const uint stringsz = str.size();

	if (stringsz == 0)
		return;

	for (uint pos = 0; pos < stringsz; pos++)
	{
		if (str[pos] == ch)
		{
			str.replace(pos, 1, replacement);
			pos += aftersz - 1;
		}
	}
}

//
// Returns true if specified character is a hex digit
//
inline bool ishexdigit(const char ch)
{
	return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

NAMESPACE_INTERNAL_END

EXTERN_C_BEGIN

#ifndef HAVE_MODFL

/**
 * Extracts signed integral and fractional values from floating-point number
 * @param x Value.
 * @param exp Pointer to the variable where integral part of a number will be stored. 
 * @returns fractional value of a floating-point number.
 */ 
long double modfl(long double x, long double * exp);

#endif

EXTERN_C_END

#endif // __CORE__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
