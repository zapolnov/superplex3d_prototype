#include <core/variant.h>
#include <core/utility.h>
#include <core/unicode.h>

#ifdef _MSC_VER
 #pragma warning(disable:4146)	// unary minus operator applied to unsigned type, result still unsigned
#endif

/* Function prototypes */

template <class CHAR> inline CHAR toLower(CHAR ch);
template <class STR, class CHAR> static inline STR convertString(const CHAR * str, unsigned long length);


/* Private functions */

//
// Converts ASCII character to the lower case
//
template <> char toLower(char ch)
{
	return tolower(ch);
}

//
// Converts wide character to the lower case
//
template <> wchar_t toLower(wchar_t ch)
{
	return (wchar_t)towlower(ch);
}

//
// Converts ASCII string to std::string
//
template <> std::string convertString(const char * str, unsigned long length)
{
	return std::string(str ? str : "", str ? length : 0);
}

//
// Converts ASCII string to std::wstring
//
template <> std::wstring convertString(const char * str, unsigned long length)
{
	return convertLocal8BitToWCHAR(str ? str : "", str ? length : 0);
}

//
// Converts wide string to std::string
//
template <> std::string convertString(const wchar_t * str, unsigned long length)
{
	return convertWCHARtoLocal8Bit(str ? str : L"", str ? length : 0);
}

//
// Converts wide string to std::wstring
//
template <> std::wstring convertString(const wchar_t * str, unsigned long length)
{
	return std::wstring(str ? str : L"", str ? length : 0);
}

//
// Converts number into string
//
template <class STR, class T>
static inline STR numberToString(T value)
{
	typename STR::value_type buf[128];
	bool sign = false;
	int pos;

	pos = (sizeof(buf) / sizeof(buf[0])) - 1;
	buf[pos--] = 0;

	if (value < 0)
	{
		sign = true;
		value = -value;
	}

	do
	{
		buf[pos--] = value % 10 + '0';
		value /= 10;
	}
	while (value != 0);

	if (sign)
		buf[pos--] = '-';

	return STR(&buf[pos + 1]);
}

//
// Converts string into number
//
template <class T, class CHAR>
static inline T stringToNumber(const CHAR * str, unsigned long length)
{
	T sign = 1, value = 0;
	unsigned long i = 0;

	if (!str || !length)
		return 0;

	if (str[i] == '-')
	{
		if (length == 1)
			return 0;

		sign = (T)(-1);
		i++;
	}

	while (i < length && str[i] >= '0' && str[i] <= '9')
		value = value * 10 + str[i] - '0';

	return value * sign;
}

//
// Convers floating-point number to string
//
template <class STR, class T>
static inline STR floatToString(T value)
{
	typename STR::value_type tmp[Internal::CVTBUFSIZE];
	bool sign = false;

	if (value < 0)
	{
		sign = true;
		value = -value;
	}	

	Internal::floatingPointToString<typename STR::value_type, T, typename STR::value_type,
		toLower<typename STR::value_type> >(value, tmp, 'g', 1);
	Internal::cropZeros(tmp);

	STR result(tmp);
	if (sign)
		return result.insert(0, 1, '-');

	return result;
}

//
// Converts string to floating-point number
//
template <class T, class CHAR>
static inline T stringToFloat(const CHAR * str, unsigned long length)
{
	T sign = 1.0, value = 0.0;
	unsigned long i = 0;

	if (!str || !length)
		return 0.0;

	if (str[i] == '-')
	{
		if (length == 1)
			return 0;

		sign = -1.0;
		i++;
	}

	while (i < length && str[i] >= '0' && str[i] <= '9')
		value = value * 10 + str[i] - '0';

	if (str[i] == '.')
	{
		T f = (T)0.0, div = (T)0.1;

		i++;
		while (i < length && str[i] >= '0' && str[i] <= '9')
		{
			f += (str[i] - '0') * div;
			div *= (T)0.1;
		}

		value += f;
	}
	else if ((str[i] == 'e' || str[i] == 'E') && i < length - 2)
	{
		i++;
		if (str[i] == '+' || str[i] == '-')
		{
			T sign = (str[i] == '-' ? (T)-1.0 : (T)1.0), power = (T)0.0;
			i++;

			while (i < length && str[i] >= '0' && str[i] <= '9')
				power = power * (T)10.0 + str[i] - '0';

			if (power != 0.0)
				value = pow(value, power * sign);
		}
	}

	return value * sign;
}


/* Variant methods */

//
// Constructor
//
Variant::Variant(const std::string & value)
	: m_Type(String)
{
	assignString(value.data(), value.length());
}

//
// Constructor
//
Variant::Variant(const std::wstring & value)
	: m_Type(WideString)
{
	assignString(value.data(), value.length());
}

//
// Constructor
//
Variant::Variant(const char * value)
	: m_Type(String)
{
	assignString(value ? value : "", value ? strlen(value) : 0);
}

//
// Constructor
//
Variant::Variant(const wchar_t * value)
	: m_Type(WideString)
{
	assignString(value ? value : L"", value ? wcslen(value) : 0);
}

//
// Assignment operator
//
Variant & Variant::operator= (const std::string & value)
{
	freeValue();
	m_Type = String;
	assignString(value.data(), value.length());
	return *this;
}

//
// Assignment operator
//
Variant & Variant::operator= (const std::wstring & value)
{
	freeValue();
	m_Type = WideString;
	assignString(value.data(), value.length());
	return *this;
}

//
// Assignment operator
//
Variant & Variant::operator= (const char * value)
{
	freeValue();
	m_Type = String;
	assignString(value ? value : "", value ? strlen(value) : 0);
	return *this;
}

//
// Assignment operator
//
Variant & Variant::operator= (const wchar_t * value)
{
	freeValue();
	m_Type = WideString;
	assignString(value ? value : L"", value ? wcslen(value) : 0);
	return *this;
}

//
// Returns value of this variant as a string
//
std::string Variant::toByteArray() const
{
	return toString<std::string>();
}

//
// Returns value of this variant as a wide string
//
std::wstring Variant::toString() const
{
	return toString<std::wstring>();
}

//
// Returns value of this variant as a signed char
//
signed char Variant::toChar() const
{
	return toNumber<signed char>();
}

//
// Returns value of this variant as an unsigned char
//
unsigned char Variant::toUChar() const
{
	return toNumber<unsigned char>();
}

//
// Returns value of this variant as a signed short integer
//
signed short Variant::toShort() const
{
	return toNumber<signed short>();
}

//
// Returns value of this variant as an unsigned short integer
//
unsigned short Variant::toUShort() const
{
	return toNumber<unsigned short>();
}

//
// Returns value of this variant as a signed integer
//
signed int Variant::toInt() const
{
	return toNumber<signed int>();
}

//
// Returns value of this variant as an unsigned integer
//
unsigned int Variant::toUInt() const
{
	return toNumber<unsigned int>();
}

//
// Returns value of this variant as a signed long integer
//
signed long Variant::toLong() const
{
	return toNumber<signed long>();
}

//
// Returns value of this variant as an unsigned long integer
//
unsigned long Variant::toULong() const
{
	return toNumber<unsigned long>();
}

//
// Returns value of this variant as a signed 64-bit integer
//
signed long long Variant::toLongLong() const
{
	return toNumber<signed long long>();
}

//
// Returns value of this variant as an unsigned 64-bit integer
//
unsigned long long Variant::toULongLong() const
{
	return toNumber<unsigned long long>();
}

//
// Returns value of this variant as a single-precision floating-point number
//
float Variant::toFloat() const
{
	return toFloatingPoint<float>();
}

//
// Returns value of this variant as a double-precision floating-point number
//
double Variant::toDouble() const
{
	return toFloatingPoint<double>();
}

//
// Returns value of this variant as a maximum-precision floating-point number
//
long double Variant::toLongDouble() const
{
	return toFloatingPoint<long double>();
}

//
// Assigns value of the specified variant to this variant
//
void Variant::assignVariant(const Variant & src)
{
	m_Type = src.m_Type;
	switch (m_Type)
	{
	case String:
		assignString((char *)src.m_Value._string.data, src.m_Value._string.length);
		break;

	case WideString:
		assignString((wchar_t *)src.m_Value._string.data, src.m_Value._string.length);
		break;

	default:
		memcpy(&m_Value, &src.m_Value, sizeof(m_Value));
	}	
}

//
// Assigns string value to this variant
//
template <class CHAR> void Variant::assignString(const CHAR * str, size_t len)
{
	if (!str || !len)
	{
		m_Value._string.length = 0;
		m_Value._string.data = NULL;
		return;
	}

	const size_t size = (len + 1) * sizeof(CHAR);

	m_Value._string.data = malloc(size);
	if (!m_Value._string.data)
		m_Value._string.length = 0;
	else
	{
		m_Value._string.length = len;
		memcpy(m_Value._string.data, str, size);
	}
}

//
// Frees any currently stored value
//
void Variant::freeValue()
{
	if (m_Type == String || m_Type == WideString)
	{
		if (m_Value._string.data)
			free(m_Value._string.data);
		m_Value._string.data = NULL;
		m_Value._string.length = 0;
	}
}

//
// Converts variant to the specified numeric type
//
template <class T> T Variant::toNumber() const
{
	switch (m_Type)
	{
	case Unknown:
		return 0;

	case String:
		return stringToNumber<T>((char *)m_Value._string.data, m_Value._string.length);

	case WideString:
		return stringToNumber<T>((wchar_t *)m_Value._string.data, m_Value._string.length);

	case UChar:
		return (T)m_Value._uchar;

	case Char:
		return (T)m_Value._char;

	case UShort:
		return (T)m_Value._ushort;

	case Short:
		return (T)m_Value._short;

	case UInt:
		return (T)m_Value._uint;

	case Int:
		return (T)m_Value._int;

	case ULong:
		return (T)m_Value._ulong;

	case Long:
		return (T)m_Value._long;

	case ULongLong:
		return (T)m_Value._ulonglong;

	case LongLong:
		return (T)m_Value._longlong;

	case Float:
		return (T)m_Value._float;

	case Double:
		return (T)m_Value._double;

	case LongDouble:
		return (T)m_Value._longdouble;
	}

	Q_ASSERT(false);
	return 0;
}

//
// Converts variant to the specified floating-point type
//
template <class T> T Variant::toFloatingPoint() const
{
	switch (m_Type)
	{
	case Unknown:
		return 0.0;

	case String:
		return stringToFloat<T>((char *)m_Value._string.data, m_Value._string.length);

	case WideString:
		return stringToFloat<T>((wchar_t *)m_Value._string.data, m_Value._string.length);

	case UChar:
		return (T)m_Value._uchar;

	case Char:
		return (T)m_Value._char;

	case UShort:
		return (T)m_Value._ushort;

	case Short:
		return (T)m_Value._short;

	case UInt:
		return (T)m_Value._uint;

	case Int:
		return (T)m_Value._int;

	case ULong:
		return (T)m_Value._ulong;

	case Long:
		return (T)m_Value._long;

	case ULongLong:
		return (T)m_Value._ulonglong;

	case LongLong:
		return (T)m_Value._longlong;

	case Float:
		return (T)m_Value._float;

	case Double:
		return (T)m_Value._double;

	case LongDouble:
		return (T)m_Value._longdouble;
	}

	Q_ASSERT(false);
	return 0.0;
}

//
// Converts variant to the specified string type
//
template <class T> T Variant::toString() const
{
	switch (m_Type)
	{
	case Unknown:
		return T();

	case String:
		return convertString<T>((char *)m_Value._string.data, m_Value._string.length);

	case WideString:
		return convertString<T>((wchar_t *)m_Value._string.data, m_Value._string.length);

	case UChar:
		return numberToString<T>(m_Value._uchar);

	case Char:
		return numberToString<T>(m_Value._char);

	case UShort:
		return numberToString<T>(m_Value._ushort);

	case Short:
		return numberToString<T>(m_Value._short);

	case UInt:
		return numberToString<T>(m_Value._uint);

	case Int:
		return numberToString<T>(m_Value._int);

	case ULong:
		return numberToString<T>(m_Value._ulong);

	case Long:
		return numberToString<T>(m_Value._long);

	case ULongLong:
		return numberToString<T>(m_Value._ulonglong);

	case LongLong:
		return numberToString<T>(m_Value._longlong);

	case Float:
		return floatToString<T>(m_Value._float);

	case Double:
		return floatToString<T>(m_Value._double);

	case LongDouble:
		return floatToString<T>(m_Value._longdouble);
	}

	Q_ASSERT(false);
	return T();
}
