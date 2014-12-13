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
#ifndef __MATH__VECTOR4_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__VECTOR4_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>


/* Classes */

/** General purpose float quad. Uses include representation of color coordinates. */
class Vector4
{
public:
	/** Type of Vec class.*/
	typedef float VALUE_TYPE;

	/** Number of vector components. */
	enum { num_components = 4 };

	/** Vec member variable. */
	VALUE_TYPE _v[4];

	/** Default constructor. */
	Vector4()
	{
		_v[0] = 0.0f;
		_v[1] = 0.0f;
		_v[2] = 0.0f;
		_v[3] = 0.0f;
	}

	/**
	 * Constructor from values.
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param z Z coordinate.
	 * @param w W coordinate.
	 */
	Vector4(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z, VALUE_TYPE w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	/**
	 * Constructor from 3-component vector and value.
	 * @param v3 3-component vector.
	 * @param w W coordinate.
	 */
	Vector4(const Vector3 & v3, VALUE_TYPE w)
	{
		_v[0] = v3[0];
		_v[1] = v3[1];
		_v[2] = v3[2];
		_v[3] = w;
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator==(const Vector4 & v) const
	{
		return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator!=(const Vector4 & v) const
	{
		return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3];
	}

	/**
	 * Comparison operator.
	 * @param v Vector to compare with.
	 */
	inline bool operator<(const Vector4 & v) const
	{
		if (_v[0] < v._v[0])
			return true;
		else if (_v[0] > v._v[0])
			return false;
		else if (_v[1] < v._v[1])
			return true;
		else if (_v[1] > v._v[1])
			return false;
		else if (_v[2] < v._v[2])
			return true;
		else if (_v[2] > v._v[2])
			return false;
		else
			return (_v[3] < v._v[3]);
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
	 * @param w W coordinate.
	 */	 
	inline void set(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z, VALUE_TYPE w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
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

	/** Returns reference to the W component of the vector. */
	inline VALUE_TYPE & w()
	{
		return _v[3];
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

	/** Returns value of the W component of the vector. */
	inline VALUE_TYPE w() const
	{
		return _v[3];
	}

	/** Returns reference to the X component of the vector. */
	inline VALUE_TYPE & r()
	{
		return _v[0];
	}

	/** Returns reference to the Y component of the vector. */
	inline VALUE_TYPE & g()
	{
		return _v[1];
	}

	/** Returns reference to the Z component of the vector. */
	inline VALUE_TYPE & b()
	{
		return _v[2];
	}

	/** Returns reference to the W component of the vector. */
	inline VALUE_TYPE & a()
	{
		return _v[3];
	}

	/** Returns value of the X component of the vector. */
	inline VALUE_TYPE r() const
	{
		return _v[0];
	}

	/** Returns value of the Y component of the vector. */
	inline VALUE_TYPE g() const
	{
		return _v[1];
	}

	/** Returns value of the Z component of the vector. */
	inline VALUE_TYPE b() const
	{
		return _v[2];
	}

	/** Returns value of the W component of the vector. */
	inline VALUE_TYPE a() const
	{
		return _v[3];
	}

	/** Returns color as ABGR. */
	inline unsigned int asABGR() const
	{
		return (unsigned int)clampTo((_v[0] * 255.0f), 0.0f, 255.0f) << 24 |
			   (unsigned int)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 16 |
			   (unsigned int)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 8  |
			   (unsigned int)clampTo((_v[3] * 255.0f), 0.0f, 255.0f);
	}

	/** Returns color as RGBA. */
	inline unsigned int asRGBA() const
	{
		return (unsigned int)clampTo((_v[3] * 255.0f), 0.0f, 255.0f) << 24 |
			   (unsigned int)clampTo((_v[2] * 255.0f), 0.0f, 255.0f) << 16 |
			   (unsigned int)clampTo((_v[1] * 255.0f), 0.0f, 255.0f) << 8  |
			   (unsigned int)clampTo((_v[0] * 255.0f), 0.0f, 255.0f);
	}

	/** Returns true if vector contains valid numbers. */
	inline bool valid() const
	{
		return !isNaN();
	}

	/** Returns true if any of the vector components contains invalid number. */
	inline bool isNaN() const
	{
		return ::isNaN(_v[0]) || ::isNaN(_v[1]) || ::isNaN(_v[2]) || ::isNaN(_v[3]);
	}

	/**
	 * Calculates dot product of this vector and the specified one.
	 * @param rhs Second vector.
	 */
	inline VALUE_TYPE operator*(const Vector4 & rhs) const
	{
		return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2] + _v[3] * rhs._v[3];
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector4 operator*(VALUE_TYPE rhs) const
	{
		return Vector4(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
	}

	/**
	 * Multiplies every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector4 & operator*=(VALUE_TYPE rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector4 operator/(VALUE_TYPE rhs) const
	{
		return Vector4(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs, _v[3] / rhs);
	}

	/**
	 * Divides every component of this vector by the specified scalar.
	 * @param rhs Scalar.
	 */
	inline Vector4 & operator/=(VALUE_TYPE rhs)
	{
		_v[0] /= rhs;
		_v[1] /= rhs;
		_v[2] /= rhs;
		_v[3] /= rhs;
		return *this;
	}

	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline Vector4 operator+(const Vector4 & rhs) const
	{
		return Vector4(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	/**
	 * Adds the specified vector this one.
	 * @param rhs Vector to add.
	 */
	inline Vector4 & operator+=(const Vector4 & rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline Vector4 operator-(const Vector4 & rhs) const
	{
		return Vector4(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2], _v[3] - rhs._v[3]);
	}

	/**
	 * Subtracts the specified vector from this one.
	 * @param rhs Vector to subtract.
	 */
	inline Vector4 & operator-=(const Vector4 & rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;
	}

	/** Returns the negative of this vector. */
	inline const Vector4 operator-() const
	{
		return Vector4(-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	/** Returns length of this vector. */
	inline VALUE_TYPE length() const
	{
		return sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
	}

	/** Returns length squared of this vector. */
	inline VALUE_TYPE length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
	}

	/** Normalizes the vector so that it has length unity. Returns the previous length of the vector. */
	inline VALUE_TYPE normalize()
	{
		VALUE_TYPE norm = Vector4::length();
		if (norm > 0.0f)
		{
			VALUE_TYPE inv = 1.0f / norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
			_v[3] *= inv;
		}

		return norm;
	}
};

Q_DECLARE_METATYPE(Vector4);


/* Functions */

/**
 * Computes the dot product of a (Vector3, 1.0) and a Vector4.
 * @param lhs 3-component vector.
 * @param rhs 4-component vector.
 */
inline Vector4::VALUE_TYPE operator*(const Vector3 & lhs, const Vector4 & rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + rhs[3];
}

/**
 * Computes the dot product of a Vector4 and a (Vector3, 1.0).
 * @param lhs 4-component vector.
 * @param rhs 3-component vector.
 */
inline Vector4::VALUE_TYPE operator*(const Vector4 & lhs, const Vector3 & rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3];
}

#endif // __MATH__VECTOR4_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
