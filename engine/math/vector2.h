/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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
#ifndef __MATH__VECTOR2_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__VECTOR2_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/utility.h>


/* Classes */

/** General purpose float pair. Uses include representation of texture coordinates. */
class Vector2
{
public:
	 /** Type of Vec class.*/
	typedef float VALUE_TYPE;

	/** Number of vector components. */
	enum { num_components = 2 };

	/** Vec member variable. */
	VALUE_TYPE _v[2];

	/** Default constructor. */
	Vector2()
	{
		_v[0] = 0.0f;
		_v[1] = 0.0f;
	}

	/**
	 * Constructor from values.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */
	Vector2(VALUE_TYPE x, VALUE_TYPE y)
	{
		_v[0] = x;
		_v[1] = y;
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator==(const Vector2 & v) const
	{
		return _v[0] == v._v[0] && _v[1] == v._v[1];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator!=(const Vector2 & v) const
	{
		return _v[0] != v._v[0] || _v[1] != v._v[1];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator<(const Vector2 & v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else
			return (_v[1] < v._v[1]);
	}

	/** Returns pointer to the vector data. */
	inline VALUE_TYPE * ptr()
	{
		return _v;
	}

	/** Returns pointer to the vector data. */
	inline const VALUE_TYPE * ptr() const
	{
		return _v;
	}

	/**
	 * Sets components of the vector.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 */	 
	inline void set(VALUE_TYPE x, VALUE_TYPE y)
	{
		_v[0] = x;
		_v[1] = y;
	}

	/**
	 * Provides access to the vector component by index.
	 * @param i Index.
	 */
	inline VALUE_TYPE & operator[](unsigned int i)
	{
		return _v[i];
	}

	/**
	 * Provides access to the vector component by index.
	 * @param i Index.
	 */
	inline VALUE_TYPE operator[](unsigned int i) const
	{
		return _v[i];
	}

	/** Returns reference to the X component of the vector. */
	inline VALUE_TYPE & x()
	{
		return _v[0];
	}

	/** Returns reference to the Y component of the vector. */
	inline VALUE_TYPE & y()
	{
		return _v[1];
	}

	/** Returns value of the X component of the vector. */
	inline VALUE_TYPE x() const
	{
		return _v[0];
	}

	/** Returns value of the Y component of the vector. */
	inline VALUE_TYPE y() const
	{
		return _v[1];
	}

	/** Returns true if vector contains valid numbers. */
	inline bool valid() const
	{
		return !isNaN();
	}

	/** Returns true if any of the vector components contains invalid number. */
	inline bool isNaN() const
	{
		return ::isNaN(_v[0]) || ::isNaN(_v[1]);
	}

	/**
	 * Calculates dot product of this vector and the specified one.
	 * @param rhs Second vector.
	 */
	inline VALUE_TYPE operator*(const Vector2 & rhs) const
	{
		return _v[0] * rhs._v[0] + _v[1] * rhs._v[1];
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline const Vector2 operator*(VALUE_TYPE rhs) const
	{
		return Vector2(_v[0] * rhs, _v[1] * rhs);
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector2 & operator*=(VALUE_TYPE rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		return *this;
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline const Vector2 operator/(VALUE_TYPE rhs) const
	{
		return Vector2(_v[0] / rhs, _v[1] / rhs);
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector2 & operator/=(VALUE_TYPE rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		return * this;
	}

	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline const Vector2 operator+(const Vector2 & rhs) const
	{
		return Vector2(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
	}

	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline Vector2 & operator+=(const Vector2 & rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		return *this;
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline const Vector2 operator-(const Vector2 & rhs) const
	{
		return Vector2(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline Vector2 & operator-=(const Vector2 & rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		return *this;
	}

	/** Returns the negative of this vector. */
	inline const Vector2 operator-() const
	{
		return Vector2(-_v[0], -_v[1]);
	}

	/** Returns length of this vector. */
	inline VALUE_TYPE length() const
	{
		return sqrtf(_v[0] * _v[0] + _v[1] * _v[1]);
	}

	/** Returns length squared of this vector. */
	inline VALUE_TYPE length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1];
	}

	/** Normalizes the vector so that it has length unity. Returns the previous length of the vector. */
	inline VALUE_TYPE normalize()
	{
		VALUE_TYPE norm = Vector2::length();

		if (norm > 0.0f)
		{
			VALUE_TYPE inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
		}

		return norm;
	}
};

Q_DECLARE_METATYPE(Vector2);

#endif // __MATH__VECTOR2_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
