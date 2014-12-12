#ifndef __CORE__COMMON_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__COMMON_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

#include <wchar.h>

/* Disable useless compiler warnings */

#ifndef __DOXYGEN__
#ifdef _MSC_VER

 #ifndef _DEBUG
  #ifndef _SECURE_SCL
   #define _SECURE_SCL				0
  #endif
  #define _HAS_ITERATOR_DEBUGGING	0
 #endif
 #ifndef _CRT_SECURE_NO_DEPRECATE
  #define _CRT_SECURE_NO_DEPRECATE	1
 #endif
 #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS	1
 #endif
 #ifndef _SCL_SECURE_NO_DEPRECATE
  #define _SCL_SECURE_NO_DEPRECATE	1
 #endif

 #pragma warning(disable:4190)		// Function has C-linkage specified but returns type which is incompatible with C
 #pragma warning(disable:4251)		// Class needs to have dll-interface to be used
 #pragma warning(disable:4275)		// Non dll-interface class used as base for dll-interface class
 #pragma warning(disable:4290)		// C++ exception specification ignored

#endif // _MSC_VER
#endif // __DOXYGEN__


/* Platform detection */

#ifndef _X86_64
 #ifdef __x86_64__
  #define _X86_64 1
 #elif defined _M_IA64
  #define _X86_64 1
 #endif
#endif


/* Includes */

#ifdef __cplusplus

 #include <typeinfo>
 #include <stdexcept>
 #include <new>
 #include <cassert>
 #include <sstream>

#endif

#include <math.h>
#include <assert.h>


/* Macros */

/**
 * Calculates size of the wide-character (see ::wchar) buffer in characters.
 * @param x wide-character buffer.
 */
#define wsizeof(x) (sizeof(x) / sizeof(wchar))

/**
 * Calculates number of elements in the static C array.
 * @param a array.
 */
#define Q_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/**
 * Compares the specified arguments (decimal or floating-point numbers) and
 * returns smaller one of them.
 * @param a First number.
 * @param b Second number.
 */
#define Q_MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * Compares the specified arguments (decimal or floating-point numbers) and
 * returns larger one of them.
 * @param a First number.
 * @param b Second number.
 */
#define Q_MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Used in "if", "while" or "for" expressions to indicate that expression will
 * most likely be true. This allows compiler to optimize the code accordingly.
 * @param x Boolean expression.
 */
#ifdef _MSC_VER
 #define likely(x)		(x)
#else
 #define likely(x)		(__builtin_expect(!!(x), 1))
#endif

/**
 * Used in "if", "while" or "for" expressions to indicate that expression will
 * most likely be false. This allows compiler to optimize the code accordingly.
 * @param x Boolean expression.
 */
#ifdef _MSC_VER
 #define unlikely(x)	(x)
#else
 #define unlikely(x)	(__builtin_expect(!!(x), 0))
#endif

/**
 * Indicates that function never returns.
 * Example of usage:
 * @code
 * NORETURN void func()
 * {
 *		exit(1);
 * }
 * @endcode
 */
#ifdef _MSC_VER
 #define NORETURN	__declspec(noreturn)
#elif defined __GNUC__
 #define NORETURN	__attribute__((noreturn))
#else
 #define NORETURN
#endif

/**
 * Used to declare unsigned enums.
 * Example of usage:
 * @code
 * enum Enum1 UNSIGNED_ENUM
 * {
 *		Value1 = 0,
 *		Value2
 * }
 * @endcode
 */
#ifdef _MSC_VER
 #define UNSIGNED_ENUM : unsigned int
#else
 #define UNSIGNED_ENUM
#endif


#ifdef Q_ASSERT
 #undef Q_ASSERT
#endif

#define Q_ASSERT(x) \
	((likely(x)) ? (void)0 : ::Internal::Sys_AssertionFailed(__FILE__, __LINE__, __FUNCTION__, #x))

#define Q_ASSERT_MSG(x, msg) \
	((likely(x)) ? (void)0 : ::Internal::Sys_AssertionFailedMsg(__FILE__, __LINE__, __FUNCTION__, msg))

#ifdef assert
 #undef assert
#endif

#define assert(x) Q_ASSERT(x)


/* Includes */

#ifdef _PG_ENABLE_QT4
 #include <Qt/qglobal.h>
#endif


/* Internal macros */

#ifndef __DOXYGEN__

 #if (!defined WIN32) && (defined __GNUC__)
  #define DLLEXPORT __attribute__((visibility("default")))
  #define DLLIMPORT __attribute__((visibility("default")))
  #define GCC_PUBLIC __attribute__((visibility("default")))
 #else
  #define DLLEXPORT __declspec(dllexport)
  #define DLLIMPORT __declspec(dllimport)
  #define GCC_PUBLIC
 #endif

 #ifdef CORE_STATIC
  #define COREAPI
 #elif defined CORE_DLL
  #define COREAPI DLLEXPORT
 #else
  #define COREAPI DLLIMPORT
 #endif

 #ifdef MATH_STATIC
  #define MATHAPI
 #elif defined MATH_DLL
  #define MATHAPI DLLEXPORT
 #else
  #define MATHAPI DLLIMPORT
 #endif

 #ifdef ENGINE_STATIC
  #define ENGINEAPI
 #elif defined ENGINE_DLL
  #define ENGINEAPI DLLEXPORT
 #else
  #define ENGINEAPI DLLIMPORT
 #endif

 #ifdef TOOLS_DLL
  #define TOOLSAPI DLLEXPORT
 #else
  #define TOOLSAPI DLLIMPORT
 #endif

 #ifdef IMAGE_STATIC
  #define IMAGEAPI
 #elif defined IMAGE_DLL
  #define IMAGEAPI DLLEXPORT
 #else
  #define IMAGEAPI DLLIMPORT
 #endif

 #ifdef MESH_STATIC
  #define MESHAPI
 #elif defined MESH_DLL
  #define MESHAPI DLLEXPORT
 #else
  #define MESHAPI DLLIMPORT
 #endif

 #ifdef SOUND_STATIC
  #define SOUNDAPI
 #elif defined SOUND_DLL
  #define SOUNDAPI DLLEXPORT
 #else
  #define SOUNDAPI DLLIMPORT
 #endif

 #ifdef GUI_STATIC
  #define GUIAPI
 #elif defined GUI_DLL
  #define GUIAPI DLLEXPORT
 #else
  #define GUIAPI DLLIMPORT
 #endif

 #ifdef INTEGRATION_DLL
  #define INTEGRATIONAPI DLLEXPORT
 #else
  #define INTEGRATIONAPI DLLIMPORT
 #endif

 #ifdef __cplusplus

  #ifdef _MSC_VER
   #define SEH_TRY		__try
   #define SEH_CATCH	__except(EXCEPTION_EXECUTE_HANDLER)
  #else
   #define SEH_TRY		try
   #define SEH_CATCH	catch(...)
  #endif

  #define NAMESPACE_INTERNAL_BEGIN	namespace Internal {
  #define NAMESPACE_INTERNAL_END	}
  #define EXTERN_C_BEGIN			extern "C" {
  #define EXTERN_C_END				}

 #else // __cplusplus

  #define NAMESPACE_INTERNAL_BEGIN
  #define NAMESPACE_INTERNAL_END
  #define EXTERN_C_BEGIN
  #define EXTERN_C_END

 #endif // __cplusplus

 #ifdef _MSC_VER
  #define snprintf _snprintf
 #endif

#endif // __DOXYGEN__


/* Assertion */

#ifdef __cplusplus

NAMESPACE_INTERNAL_BEGIN
EXTERN_C_BEGIN

/**
 * Called by the Q_ASSERT macro when assertion failure has been detected.
 * @param file Path to the source file.
 * @param line Line number in the source file
 * @param function Name of the function.
 * @param expr Failed expression.
 */
COREAPI NORETURN void Sys_AssertionFailed(const char * file, const int line,
	const char * function, const char * expr) throw(std::exception);

/**
 * Called by the Q_ASSERT_MSG macro when assertion failure has been detected.
 * @param file Path to the source file.
 * @param line Line number in the source file
 * @param function Name of the function.
 * @param message Error description.
 */
COREAPI NORETURN void Sys_AssertionFailedMsg(const char * file, const int line,
	const char * function, const char * message) throw(std::exception);


EXTERN_C_END
NAMESPACE_INTERNAL_END

#endif // __cplusplus


/* Functions */

/**
 * Template that translates into dynamic_cast<> or static_cast<>
 * depending on the compilation mode.
 */
#ifdef __cplusplus
 #ifdef _DEBUG
  template <class TO, class FROM> inline TO safe_cast(FROM inp)
  {
	if (!inp)
		return NULL;
	TO outp = dynamic_cast<TO>(inp);
	if (unlikely(!outp))
	{
		std::stringstream str;

		str << "Bad cast from " << typeid(FROM).name() << " to " << typeid(TO).name();
		Q_ASSERT_MSG(false, str.str().c_str());
	}
	return outp;
  }
 #else // _DEBUG
  #define safe_cast static_cast
 #endif // _DEBUG
#endif // __cplusplus


/* Types selector */

#ifdef __cplusplus

/*
 *
 * John Martin: john.p.martin@eudoramail.com
 *
 * This file utilizes code derived from Andrei Alexandrescu's book:
 * "Modern C++ Design, Design Patterns and Generic Programming Applied"
 * published by Addison Wesley.  His code is free to use and so is mine.
 * Enjoy.
 *
 */

#ifndef __DOXYGEN__

NAMESPACE_INTERNAL_BEGIN

namespace __autotype
{
	class null_type {};

	template <class U, class V> struct typelist
	{
		typedef U head_type;
		typedef V tail_type;
	};

	#define TL_1(T1) typelist<T1, null_type>
	#define TL_2(T1,T2) typelist<T1, TL_1(T2)>
	#define TL_3(T1,T2,T3) typelist<T1, TL_2(T2,T3)>
	#define TL_4(T1,T2,T3,T4) typelist<T1, TL_3(T2,T3,T4)>
	#define TL_5(T1,T2,T3,T4,T5) typelist<T1, TL_4(T2,T3,T4,T5)>
	
	template <bool Expr, class U, class V> struct select_type
	{
		typedef U result_type;
	};
	
	template <class U, class V> struct select_type<false, U, V>
	{
		typedef V result_type;
	};

	template <unsigned int Size, class TList> struct select_type_size;

	template <unsigned int Size, class U, class V> struct select_type_size<Size, typelist<U, V> >
	{
  		typedef typename select_type<Size == sizeof(U), U,
			typename select_type_size<Size, V>::result_type>::result_type result_type;
	};
	
	template <unsigned int Size> struct select_type_size<Size, null_type>
	{
		typedef null_type result_type;
	};
	
	typedef TL_5(signed char, signed short, signed int, signed long, signed long long) signed_ints;
	typedef TL_5(unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long) unsigned_ints;

	typedef select_type_size<1, signed_ints>::result_type int8;
	typedef select_type_size<2, signed_ints>::result_type int16;
	typedef select_type_size<4, signed_ints>::result_type int32;
	typedef select_type_size<8, signed_ints>::result_type int64;
	typedef select_type_size<1, unsigned_ints>::result_type uint8;
	typedef select_type_size<2, unsigned_ints>::result_type uint16;
	typedef select_type_size<4, unsigned_ints>::result_type uint32;
	typedef select_type_size<8, unsigned_ints>::result_type uint64;
	typedef select_type_size<sizeof(void *), signed_ints>::result_type sptr;
	typedef select_type_size<sizeof(void *), unsigned_ints>::result_type uptr;

	#undef TL_1
	#undef TL_2
	#undef TL_3
	#undef TL_4
	#undef TL_5
}

NAMESPACE_INTERNAL_END

#endif // __DOXYGEN__

/** 8-bit signed data type. */
typedef Internal::__autotype::int8 int8;
/** 16-bit signed data type. */
typedef Internal::__autotype::int16 int16;
/** 32-bit signed data type. */
typedef Internal::__autotype::int32 int32;
/** 64-bit signed data type. */
typedef Internal::__autotype::int64 int64;

/** 8-bit unsigned data type. */
typedef Internal::__autotype::uint8 uint8;
/** 16-bit unsigned data type. */
typedef Internal::__autotype::uint16 uint16;
/** 32-bit unsigned data type. */
typedef Internal::__autotype::uint32 uint32;
/** 64-bit unsigned data type. */
typedef Internal::__autotype::uint64 uint64;

/** pointer-sized signed data type. */
typedef Internal::__autotype::sptr intptr;
/** pointer-sized unsigned data type. */
typedef Internal::__autotype::uptr uintptr;

/** 8-bit unsigned data type. */
typedef uint8 byte;
/** 16-bit unsigned data type. */
typedef uint16 word;
/** 32-bit unsigned data type. */
typedef uint32 dword;

#endif // __cplusplus

/** Short equivalent of unsigned char. */
typedef unsigned char uchar;
/** Short equivalent of unsigned short. */
typedef unsigned short ushort;
/** Short equivalent of unsigned int. */
typedef unsigned int uint;
/** Short equivalent of unsigned long. */
typedef unsigned long ulong;
/** Short equivalent of long long. */
typedef long long longlong;
/** Short equivalent of unsigned long long. */
typedef unsigned long long ulonglong;
/** Short equivalent of long double. */
typedef long double ldouble;

/** Wide character. */
typedef wchar_t	wchar;

#endif // __CORE__COMMON_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
