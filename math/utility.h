/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
#ifndef __MATH__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#if defined(_MSC_VER) || defined(sun) || defined(__APPLE__)
 #include <float.h>
#endif


/* Macros */

#ifdef __APPLE__
 #include <cmath>
 #include <AvailabilityMacros.h>
 #if !defined(MAC_OS_X_VERSION_10_2) || (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_2)
  #if (__GNUC__ < 4)
   #define APPLE_PRE_10_2
  #endif
 #endif
#endif


/* Constants */

#ifdef PI
 #undef PI
 #undef PI_2
 #undef PI_4
#endif

const double PI			= 3.14159265358979323846;
const double PI_2		= 1.57079632679489661923;
const double PI_4		= 0.78539816339744830962;
const double LN_2		= 0.69314718055994530942;
const double INVLN_2	= 1.0 / LN_2;


/* Functions */

#if defined(sun) || defined(__APPLE__)

#ifndef acosf
inline float acosf(float value)
{
	return static_cast<float>(acos(value));
}
#endif

#ifndef asinf
inline float asinf(float value)
{
	return static_cast<float>(asin(value));
}
#endif

#ifndef cosf
inline float cosf(float value)
{
	return static_cast<float>(cos(value));
}
#endif

#ifndef sinf
inline float sinf(float value)
{
	return static_cast<float>(sin(value));
}
#endif

#ifndef logf
inline float logf(float value)
{
	return static_cast<float>(log(value));
}
#endif

#ifndef powf
inline float powf(float value1, float value2)
{
	return static_cast<float>(pow(value1, value2));
}
#endif

#ifndef sqrtf
inline float sqrtf(float value)
{
	return static_cast<float>(sqrt(value));
}
#endif

#ifndef fabsf
inline float fabsf(float value)
{
	return static_cast<float>(fabs(value));
}
#endif

#ifndef atan2f
inline float atan2f(float value1, float value2)
{
	return static_cast<float>(atan2(value1, value2));
}
#endif

#ifndef tanf
inline float tanf(float value)
{
	return static_cast<float>(tan(value));
}
#endif

#endif // defined(sun) || defined(__APPLE__)

#if defined(sun) || defined(__hpux) || defined(APPLE_PRE_10_2)

#ifndef floorf
inline float floorf(float value)
{
	return static_cast<float>(floor(value));
}
#endif

#ifndef ceilf
inline float ceilf(float value)
{
	return static_cast<float>(ceil(value));
}
#endif

#endif // defined(sun) || defined(__hpux) || defined(APPLE_PRE_10_2)

/**
 * Returns the absolute value of the specified value.
 * @param v Value to return absolute value of.
 */
template <typename T> inline
T absolute(T v)
{
	return v < (T)0 ? -v : v;
}

/**
 * Return true if float lhs and rhs are equivalent,
 * meaning that the difference between them is less than an epsilon value
 * which defaults to 1e-6.
 */
inline
bool equivalent(float lhs, float rhs, float epsilon = 1e-6)
{
	float delta = rhs - lhs;
	return delta < 0.0f ? delta >= -epsilon : delta <= epsilon;
}

/**
 * Return true if double lhs and rhs are equivalent,
 * meaning that the difference between them is less than an epsilon value
 * which defaults to 1e-6.
 */
inline
bool equivalent(double lhs, double rhs, double epsilon = 1e-6)
{
	double delta = rhs - lhs;
	return delta < 0.0 ? delta >= -epsilon : delta <= epsilon;
}

/**
 * Returns the minimum of two values, equivalent to std::min.
 * std::min not used because of STL implementation under IRIX not containing
 * std::min.
 */
template <typename T> inline
T minimum(T lhs,T rhs)
{
	return lhs < rhs ? lhs : rhs;
}

/**
 * Return the maximum of two values, equivalent to std::max.
 * std::max not used because of STL implementation under IRIX not containing
 * std::max.
 */
template <typename T> inline
T maximum(T lhs, T rhs)
{
	return lhs > rhs ? lhs : rhs;
}

/**
 * Returns value clamped to the specified range.
 * @param v Value to clamp.
 * @param minimum Minimum allowed value.
 * @param maximum Maximum allowed value.
 */
template <typename T> inline
T clampTo(T v, T minimum, T maximum)
{
	return v < minimum ? minimum : v > maximum ? maximum : v;
}

/** Returns the minimum of two values. */
template <typename T> inline
T clampAbove(T v, T minimum)
{
	return v < minimum ? minimum : v;
}

/** Returns the maximum of two values. */
template <typename T> inline
T clampBelow(T v, T maximum)
{
	return v > maximum ? maximum : v;
}

/**
 * Returns value clamped to the specified range.
 * @param v Value to clamp.
 * @param minimum Minimum allowed value.
 * @param maximum Maximum allowed value.
 */
template <typename T> inline
T clampBetween(T v, T minimum, T maximum)
{
	return clampBelow(clampAbove(v, minimum), maximum);
}

/** Returns sign of the specified value. */
template <typename T> inline
T sign(T v)
{
	return v < (T)0 ? (T)-1 : (T)1;
}

/** Returns sign of the specified value or 0 if it is equal to zero. */
template <typename T> inline
T signOrZero(T v)
{
	return v < (T)0 ? (T)-1 : (v > (T)0 ? (T)1 : 0 );
}

/** Returns squared value. */
template <typename T> inline
T square(T v)
{
	return v * v;
}

/** Returns squared value with preserved sign. */
template <typename T> inline
T signedSquare(T v)
{
	return v < (T)0 ? -v * v : v * v;
}

/** Converts angle from degress into radians. */
inline float inDegrees(float angle)
{
	return angle * (float)PI / 180.0f;
}

/** Converts angle from degrees into radians. */
inline double inDegrees(double angle)
{
	return angle * PI / 180.0;
}

/** Converts angle from radians into radians. */
template <typename T> inline
T inRadians(T angle)
{
	return angle;
}

/** Converts angle from degress into radians. */
inline float degreesToRadians(float angle)
{
	return angle * (float)PI / 180.0f;
}

/** Converts angle from degress into radians. */
inline double DegreesToRadians(double angle)
{
	return angle * PI / 180.0;
}

/** Converts angle from radians into degrees. */
inline float RadiansToDegrees(float angle)
{
	return angle * 180.0f / (float)PI;
}

/** Converts angle from radians into degrees. */
inline double RadiansToDegrees(double angle)
{
	return angle * 180.0 / PI;
}

/** Rounds the specified value. */
inline float round(float v)
{
	return v >= 0.0f ? floorf(v + 0.5f) : ceilf(v - 0.5f);
}

/** Rounds the specified value. */
inline double round(double v)
{
	return v >= 0.0 ? floor(v + 0.5) : ceil(v - 0.5);
}

#if defined(WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)

inline bool isNaN(float v)
{
	return _isnan(v) != 0;
}

inline bool isNaN(double v)
{
	return _isnan(v) != 0;
}

#else // defined(WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)

#ifdef __APPLE__

inline bool isNaN(float v)
{
	return std::isnan(v);
}

inline bool isNaN(double v)
{
	return std::isnan(v);
}

#else // __APPLE__

inline bool isNaN(float v)
{
	return isnan(v);
}

inline bool isNaN(double v)
{
	return isnan(v);
}

#endif // __APPLE__

#endif // defined(WIN32) && !defined(__CYGWIN__) && !defined(__MWERKS__)

/** Computes the volume of a tetrahedron. */
template <typename T> inline
float computeTetrahedronVolume(const T & a,const T & b,const T & c,const T & d)
{
	return fabsf(((b - c) ^ (a - b)) * (d - b));
}

/** Compute the volume of a prism. */
template <typename T> inline
float computePrismVolume(const T & f1, const T & f2,const T & f3,
	const T & b1,const T & b2,const T & b3)
{
	return computeTetrahedronVolume(f1, f2, f3, b1) +
		   computeTetrahedronVolume(b1, b2, b3, f2) +
		   computeTetrahedronVolume(b1, b3, f2, f3);
}

#endif  // __MATH__UTILITY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
