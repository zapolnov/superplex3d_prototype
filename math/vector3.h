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
#ifndef __MATH__VECTOR3_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__VECTOR3_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector2.h>
#include <math/utility.h>


/* Classes */

/**
 * General purpose float triple for use as vertices, vectors and normals.
 * Provides general math operations from addition through to cross products.
 */
class Vector3
{
public:
	/** Type of vector components.*/
	typedef float VALUE_TYPE;

	/** Number of vector components. */
	enum { num_components = 3 };

	/** Vector components. */
	VALUE_TYPE _v[3];

	/** Default constructor. */
	Vector3()
	{
		_v[0] = 0.0f;
		_v[1] = 0.0f;
		_v[2] = 0.0f;
	}

	/**
	 * Constructor from values.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param z Z coordinate.
	 */
	Vector3(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
	}

	/**
	 * Constructor from 2-component vector and value.
	 * @param v2 2-component vector.
	 * @param zz Z coordinate.
	 */
	Vector3(const Vector2 & v2, VALUE_TYPE zz)
	{
		_v[0] = v2[0];
		_v[1] = v2[1];
		_v[2] = zz;
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator==(const Vector3 & v) const
	{
		return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator!=(const Vector3 & v) const
	{
		return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator<(const Vector3 & v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else if (_v[1] < v._v[1])
			return true;
		else if (_v[1] > v._v[1])
			return false;
		else
			return (_v[2] < v._v[2]);
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
	 * @param z Z coordinate.
	 */
	inline void set(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
	}

	/**
	 * Sets components of the vector.
	 * @param rhs Source vector.
	 */	 
	inline void set(const Vector3 & rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		_v[2] = rhs._v[2];
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

	/** Returns reference to the Z component of the vector. */
	inline VALUE_TYPE & z()
	{
		return _v[2];
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

	/** Returns value of the Z component of the vector. */
	inline VALUE_TYPE z() const
	{
		return _v[2];
	}

	/** Returns true if vector contains valid numbers. */
	inline bool valid() const
	{
		return !isNaN();
	}

	/** Returns true if any of the vector components contains invalid number. */
	inline bool isNaN() const
	{
		return ::isNaN(_v[0]) || ::isNaN(_v[1]) || ::isNaN(_v[2]);
	}

	/**
	 * Calculates dot product of this vector and the specified one.
	 * @param rhs Second vector.
	 */
	inline VALUE_TYPE operator*(const Vector3 & rhs) const
	{
		return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2];
	}

	/**
	 * Calculates cross product of this vector and the specified one.
	 * @param rhs Second vector.
	 */
	inline const Vector3 operator^(const Vector3 & rhs) const
	{
		return Vector3(
			_v[1] * rhs._v[2] - _v[2] * rhs._v[1],
			_v[2] * rhs._v[0] - _v[0] * rhs._v[2],
			_v[0] * rhs._v[1] - _v[1] * rhs._v[0]);
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline const Vector3 operator*(VALUE_TYPE rhs) const
	{
		return Vector3(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs);
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector3 & operator*=(VALUE_TYPE rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		return *this;
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline const Vector3 operator/(VALUE_TYPE rhs) const
	{
		return Vector3(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs);
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector3 & operator/=(VALUE_TYPE rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		_v[2] /= rhs;
		return *this;
	}

	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline const Vector3 operator+(const Vector3 & rhs) const
	{
		return Vector3(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2]);
	}
	
	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline Vector3 & operator+=(const Vector3 & rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		return *this;
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline const Vector3 operator-(const Vector3 & rhs) const
	{
		return Vector3(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2]);
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline Vector3 & operator-=(const Vector3 & rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		return *this;
	}

	/** Returns the negative of this vector. */
	inline const Vector3 operator-() const
	{
		return Vector3(-_v[0], -_v[1], -_v[2]);
	}

	/** Returns length of this vector. */
	inline VALUE_TYPE length() const
	{
		return sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
	}

	/** Returns length squared of this vector. */
	inline VALUE_TYPE length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
	}

	/** Normalizes the vector so that it has length unity. Returns the previous length of the vector. */
	inline VALUE_TYPE normalize()
	{
		VALUE_TYPE norm = Vector3::length();

		if (norm > 0.0f)
		{
			VALUE_TYPE inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
		}                

		return norm;
	}
};

Q_DECLARE_METATYPE(Vector3);


/* Constants */

const Vector3 X_AXIS(1.0, 0.0, 0.0);
const Vector3 Y_AXIS(0.0, 1.0, 0.0);
const Vector3 Z_AXIS(0.0, 0.0, 1.0);


/* Operators */

/**
 * Multiplication operator.
 * @param a First argument.
 * @param b Second argument.
 */
inline static
Vector3 operator*(float a, const Vector3 & b)
{
	return Vector3(a * b.x(), a * b.y(), a * b.z());
}

/**
 * Division operator.
 * @param a First argument.
 * @param b Second argument.
 */
inline static
Vector3 operator/(float a, const Vector3 & b)
{
	return Vector3(a / b.x(), a / b.y(), a / b.z());
}

#endif // __MATH__VECTOR3_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
