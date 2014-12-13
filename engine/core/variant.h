#ifndef __CORE__VARIANT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__VARIANT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>


/* Classes */

/** Class that allows to store values of different types. */
class Variant
{
public:
	/** Type of the variant. */
	enum Type
	{
		Unknown,	/**< Unknown value type. */
		UChar,		/**< Unsigned char. */
		Char,		/**< Char. */
		UShort,		/**< Unsigned short. */
		Short,		/**< Short. */
		UInt,		/**< Unsigned int. */
		Int,		/**< int. */
		ULong,		/**< Unsigned long. */
		Long,		/**< long. */
		ULongLong,	/**< Unsigned long long. */
		LongLong,	/**< long long. */
		Float,		/**< float. */
		Double,		/**< double. */
		LongDouble,	/**< long double. */
		String,		/**< std::string. */
		WideString,	/**< std::wstring. */
	};

	/** Constructor. */
	inline Variant()
		: m_Type(Unknown)
	{
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(unsigned char value)
		: m_Type(UChar)
	{
		m_Value._uchar = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(signed char value)
		: m_Type(Char)
	{
		m_Value._char = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(unsigned short value)
		: m_Type(UShort)
	{
		m_Value._ushort = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(signed short value)
		: m_Type(Short)
	{
		m_Value._short = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(unsigned int value)
		: m_Type(UInt)
	{
		m_Value._uint = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(signed int value)
		: m_Type(Int)
	{
		m_Value._int = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(unsigned long value)
		: m_Type(ULong)
	{
		m_Value._ulong = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(signed long value)
		: m_Type(Long)
	{
		m_Value._long = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(unsigned long long value)
		: m_Type(ULongLong)
	{
		m_Value._ulonglong = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(signed long long value)
		: m_Type(LongLong)
	{
		m_Value._longlong = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(float value)
		: m_Type(Float)
	{
		m_Value._float = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(double value)
		: m_Type(Double)
	{
		m_Value._double = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	inline Variant(long double value)
		: m_Type(LongDouble)
	{
		m_Value._longdouble = value;
	}

	/**
	 * Constructor.
	 * @param value Value.
	 */
	COREAPI Variant(const std::string & value);

	/**
	 * Constructor.
	 * @param value Value.
	 */
	COREAPI Variant(const std::wstring & value);

	/**
	 * Constructor.
	 * @param value Value.
	 */
	COREAPI Variant(const char * value);

	/**
	 * Constructor.
	 * @param value Value.
	 */
	COREAPI Variant(const wchar_t * value);

	/**
	 * Copy constructor.
	 * @param src Reference to the source value.
	 */
	inline Variant(const Variant & src)
	{
		assignVariant(src);
	}

	/** Destructor. */
	inline ~Variant()
	{
		freeValue();
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (unsigned char value)
	{
		freeValue();
		m_Type = UChar;
		m_Value._uchar = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (signed char value)
	{
		freeValue();
		m_Type = Char;
		m_Value._char = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (unsigned short value)
	{
		freeValue();
		m_Type = UShort;
		m_Value._ushort = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (signed short value)
	{
		freeValue();
		m_Type = Short;
		m_Value._short = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (unsigned int value)
	{
		freeValue();
		m_Type = UInt;
		m_Value._uint = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (signed int value)
	{
		freeValue();
		m_Type = Int;
		m_Value._int = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (unsigned long value)
	{
		freeValue();
		m_Type = ULong;
		m_Value._ulong = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (signed long value)
	{
		freeValue();
		m_Type = Long;
		m_Value._long = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (unsigned long long value)
	{
		freeValue();
		m_Type = ULongLong;
		m_Value._ulonglong = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (signed long long value)
	{
		freeValue();
		m_Type = LongLong;
		m_Value._longlong = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (float value)
	{
		freeValue();
		m_Type = Float;
		m_Value._float = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (double value)
	{
		freeValue();
		m_Type = Double;
		m_Value._double = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	inline Variant & operator= (long double value)
	{
		freeValue();
		m_Type = LongDouble;
		m_Value._longdouble = value;
		return *this;
	}

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	COREAPI Variant & operator= (const std::string & value);

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	COREAPI Variant & operator= (const std::wstring & value);

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	COREAPI Variant & operator= (const char * value);

	/**
	 * Assignment operator.
	 * @param value Value to assign.
	 */
	COREAPI Variant & operator= (const wchar_t * value);

	/**
	 * Assignment operator.
	 * @param src Reference to the source value.
	 */
	inline Variant & operator= (const Variant & src)
	{
		freeValue();
		assignVariant(src);
		return *this;
	}

	/** Returns type of the variant. */
	inline Type type() const
	{
		return m_Type;
	}

	/** Returns value of this variant as a string. */
	COREAPI std::string toByteArray() const;

	/** Returns value of this variant as a wide string. */
	COREAPI std::wstring toString() const;

	/** Returns value of this variant as a signed char. */
	COREAPI signed char toChar() const;

	/** Returns value of this variant as an unsigned char. */
	COREAPI unsigned char toUChar() const;

	/** Returns value of this variant as a signed short integer. */
	COREAPI signed short toShort() const;

	/** Returns value of this variant as an unsigned short integer. */
	COREAPI unsigned short toUShort() const;

	/** Returns value of this variant as a signed integer. */
	COREAPI signed int toInt() const;

	/** Returns value of this variant as an unsigned integer. */
	COREAPI unsigned int toUInt() const;

	/** Returns value of this variant as a signed long integer. */
	COREAPI signed long toLong() const;

	/** Returns value of this variant as an unsigned long integer. */
	COREAPI unsigned long toULong() const;

	/** Returns value of this variant as a signed 64-bit integer. */
	COREAPI signed long long toLongLong() const;

	/** Returns value of this variant as an unsigned 64-bit integer. */
	COREAPI unsigned long long toULongLong() const;

	/** Returns value of this variant as a single-precision floating-point number. */
	COREAPI float toFloat() const;

	/** Returns value of this variant as a double-precision floating-point number. */
	COREAPI double toDouble() const;

	/** Returns value of this variant as a maximum-precision floating-point number. */
	COREAPI long double toLongDouble() const;

private:
	/** Value storage. */
	union Value
	{
		unsigned char _uchar;			/**< Storage for unsigned char. */
		signed char _char;				/**< Storage for char. */
		unsigned short _ushort;			/**< Storage for unsigned short. */
		signed short _short;			/**< Storage for short. */
		unsigned int _uint;				/**< Storage for unsigned int. */
		signed int _int;				/**< Storage for int. */
		unsigned long _ulong;			/**< Storage for unsigned long. */
		signed long _long;				/**< Storage for long. */
		unsigned long long _ulonglong;	/**< Storage for unsigned long long. */
		signed long long _longlong;		/**< Storage for long long. */
		float _float;					/**< Storage for float. */
		double _double;					/**< Storage for double. */
		double _longdouble;				/**< Storage for double. */
		struct
		{
			void * data;				/**< Pointer to the string data. */
			unsigned long length;		/**< Length of the string. */
		} _string;						/**< Storage for string. */
	};

	Type m_Type;				/**< Type of the stored value. */
	Value m_Value;				/**< Stored value. */
	char m_Buffer;				/**< Temporary variable used for MySQL binding. */
	unsigned long m_Length;		/**< Temporary variable used for MySQL binding. */

	/**
	 * Assigns value of the specified variant to this variant.
	 * @param src Reference to the source variant.
	 */
	COREAPI void assignVariant(const Variant & src);

	/**
	 * Assigns string value to this variant.
	 * @param str Pointer to the string.
	 * @param len Length of the string.
	 */
	template <class CHAR> void assignString(const CHAR * str, size_t len);

	/** Frees any currently stored value. */
	COREAPI void freeValue();

	/** Converts variant to the specified numeric type. */
	template <class T> inline T toNumber() const;

	/** Converts variant to the specified floating-point type. */
	template <class T> inline T toFloatingPoint() const;

	/** Converts variant to the specified string type. */
	template <class T> inline T toString() const;
};

#endif // __CORE__VARIANT_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
