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
#ifndef __MATH__QUATERNION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__QUATERNION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>
#include <math/vector4.h>


/* Classes */

class Matrix;

/** A quaternion class. It can be used to represent an orientation in 3D space. */
class Quaternion
{
public:
	/** Type of the component. */
	typedef float VALUE_TYPE;

	/** Number of quaternion components. */
	enum { num_components = 4 };

	/** Member variable. */
	VALUE_TYPE _v[4];

	/** Default constructor. */
	inline Quaternion()
	{
		_v[0] = 0.0;
		_v[1] = 0.0;
		_v[2] = 0.0;
		_v[3] = 1.0;
	}

	/**
	 * Constructor from values.
	 * @param x X component.
	 * @param y Y component.
	 * @param z Z component.
	 * @param w W component.
	 */
	inline Quaternion(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z, VALUE_TYPE w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	/**
	 * Constructor from 4-component vector.
	 * @param v Source vector.
	 */
	inline Quaternion(const Vector4 & v)
	{
		_v[0] = v._v[0];
		_v[1] = v._v[1];
		_v[2] = v._v[2];
		_v[3] = v._v[3];
	}

	/**
	 * Constructor from axis and angle.
	 * @param angle Rotation angle.
	 * @param axis Rotation axis.
	 */
	inline Quaternion(VALUE_TYPE angle, const Vector3 & axis)
	{
		makeRotate(angle, axis);
	}

	/**
	 * Constructor from three rotations.
	 * @param angle1 Rotation angle 1.
	 * @param axis2 Rotation axis 1.
	 * @param angle2 Rotation angle 2.
	 * @param axis2 Rotation axis 2.
	 * @param angle3 Rotation angle 3.
	 * @param axis3 Rotation axis 3.
	 */
	inline Quaternion(VALUE_TYPE angle1, const Vector3 & axis1,
		VALUE_TYPE angle2, const Vector3 & axis2,
		VALUE_TYPE angle3, const Vector3 & axis3)
	{
		makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	}

	/**
	 * Assignment operator.
	 * @param v Source quaternion.
	 */
	inline Quaternion & operator=(const Quaternion & v)
	{
		_v[0] = v._v[0];
		_v[1] = v._v[1];
		_v[2] = v._v[2];
		_v[3] = v._v[3];
		return *this;
	}

	/**
	 * Sets value of the quaternion from 4-component vector.
	 * @param v Source vector.
	 */
	inline void set(const Vector4 & v)
	{
		_v[0] = v._v[0];
		_v[1] = v._v[1];
		_v[2] = v._v[2];
		_v[3] = v._v[3];
	}

	/**
	 * Sets value of the quaternion.
	 * @param x X component.
	 * @param y Y component.
	 * @param z Z component.
	 * @param w W component.
	 */
	inline void set(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z, VALUE_TYPE w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	/**
	 * Provides access to the quaternion component by index.
	 * @param i Index.
	 */
	inline VALUE_TYPE & operator[](unsigned int i)
	{
		return _v[i];
	}

	/**
	 * Provides access to the quaternion component by index.
	 * @param i Index.
	 */
	inline VALUE_TYPE operator[](unsigned int i) const
	{
		return _v[i];
	}

	/** Returns reference to the X component of the quaternion. */
	inline VALUE_TYPE & x()
	{
		return _v[0];
	}

	/** Returns reference to the Y component of the quaternion. */
	inline VALUE_TYPE & y()
	{
		return _v[1];
	}

	/** Returns reference to the Z component of the quaternion. */
	inline VALUE_TYPE & z()
	{
		return _v[2];
	}

	/** Returns reference to the W component of the quaternion. */
	inline VALUE_TYPE & w()
	{
		return _v[3];
	}

	/** Returns value of the X component of the quaternion. */
	inline VALUE_TYPE x() const
	{
		return _v[0];
	}

	/** Returns value of the Y component of the quaternion. */
	inline VALUE_TYPE y() const
	{
		return _v[1];
	}

	/** Returns value of the Z component of the quaternion. */
	inline VALUE_TYPE z() const
	{
		return _v[2];
	}

	/** Returns value of the W component of the quaternion. */
	inline VALUE_TYPE w() const
	{
		return _v[3];
	}

	/**
	 * Comparison operator.
	 * @param v Quaternion to compare with.
	 */
	inline bool operator==(const Quaternion & v) const
	{
		return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3];
	}

	/**
	 * Comparison operator.
	 * @param v Quaternion to compare with.
	 */
	inline bool operator!=(const Quaternion & v) const
	{
		return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3];
	}

	/**
	 * Comparison operator.
	 * @param v Quaternion to compare with.
	 */
	inline bool operator<(const Quaternion & v) const
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

	/** Converts quaternion into 4-component vector. */
	inline Vector4 asVec4() const
	{
		return Vector4(_v[0], _v[1], _v[2], _v[3]);
	}

	/** Converts quaternion into 3-component vector. */
	inline Vector3 asVec3() const
	{
		return Vector3(_v[0], _v[1], _v[2]);
	}

	/**
	 * Sets quaternion from the specified matrix.
	 * @param m Source matrix.
	 */
	MATHAPI void setFromMatrix(const Matrix & m);

	/**
	 * Sets rotation component of the specified matrix to the rotation described by this quaternion.
	 * @param m Target matrix.
	 */
	MATHAPI void getMatrix(Matrix & m) const;

	/** Return true if the quaternion represents a zero rotation, and therefore can be ignored in computations.*/
	bool zeroRotation() const
	{
		return _v[0] == 0.0f && _v[1] == 0.0f && _v[2] == 0.0f && _v[3] == 1.0f;
	}

	/**
	 * Multiplies this quaternion by the specified scalar.
	 * @param rhs Scalar to multiply by.
	 */
	inline Quaternion operator*(VALUE_TYPE rhs) const
	{
		return Quaternion(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
	}

	/**
	 * Multiplies this quaternion by the specified scalar.
	 * @param rhs Scalar to multiply by.
	 */
	inline Quaternion & operator*=(VALUE_TYPE rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;
	}

	/**
	 * Multiplies this quaternion by another one.
	 * @param rhs Quaternion to multiply by.
	 */
	inline const Quaternion operator*(const Quaternion & rhs) const
	{
		return Quaternion
		(
			_v[1] * rhs._v[2] - _v[2] * rhs._v[1] + _v[3] * rhs._v[0] + _v[0] * rhs._v[3],
			_v[2] * rhs._v[0] - _v[0] * rhs._v[2] + _v[3] * rhs._v[1] + _v[1] * rhs._v[3],
			_v[0] * rhs._v[1] - _v[1] * rhs._v[0] + _v[3] * rhs._v[2] + _v[2] * rhs._v[3],
			_v[3] * rhs._v[3] - _v[0] * rhs._v[0] - _v[1] * rhs._v[1] - _v[2] * rhs._v[2]
		);
	}

	/**
	 * Multiplies this quaternion by another one.
	 * @param rhs Quaternion to multiply by.
	 */
	inline Quaternion & operator*=(const Quaternion & rhs)
	{
		VALUE_TYPE x = _v[1] * rhs._v[2] - _v[2] * rhs._v[1] + _v[3] * rhs._v[0] + _v[0] * rhs._v[3];
		VALUE_TYPE y = _v[2] * rhs._v[0] - _v[0] * rhs._v[2] + _v[3] * rhs._v[1] + _v[1] * rhs._v[3];
		VALUE_TYPE z = _v[0] * rhs._v[1] - _v[1] * rhs._v[0] + _v[3] * rhs._v[2] + _v[2] * rhs._v[3];
		_v[3] = _v[3] * rhs._v[3] - _v[0] * rhs._v[0] - _v[1] * rhs._v[1] - _v[2] * rhs._v[2];
		_v[2] = z;
		_v[1] = y;
		_v[0] = x;

		return *this;
	}

	/**
	 * Divides this quaternion by the specified scalar.
	 * @param rhs Scalar to divide by.
	 */
	inline Quaternion operator/(VALUE_TYPE rhs) const
	{
		VALUE_TYPE div = 1.0f / rhs;
		return Quaternion(_v[0] * div, _v[1] * div, _v[2] * div, _v[3] * div);
	}

	/**
	 * Divides this quaternion by the specified scalar.
	 * @param rhs Scalar to divide by.
	 */
	inline Quaternion & operator/=(VALUE_TYPE rhs)
	{
		VALUE_TYPE div = 1.0f / rhs;
		_v[0] *= div;
		_v[1] *= div;
		_v[2] *= div;
		_v[3] *= div;
		return *this;
	}

	/**
	 * Divides this quaternion by another one.
	 * @param rhs Quaternion to divide by.
	 */
	inline Quaternion operator/(const Quaternion & denom) const
	{
		return (*this * denom.inverse());
	}

	/**
	 * Divides this quaternion by another one.
	 * @param rhs Quaternion to divide by.
	 */
	inline Quaternion & operator/=(const Quaternion & denom)
	{
		*this = *this * denom.inverse();
		return *this;
	}

	/**
	 * Adds another quaternion to this one.
	 * @param rhs Quaternion to add.
	 */
	inline Quaternion operator+(const Quaternion & rhs) const
	{
		return Quaternion(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	/**
	 * Adds another quaternion to this one.
	 * @param rhs Quaternion to add.
	 */
	inline Quaternion & operator+=(const Quaternion & rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;
	}

	/**
	 * Subtracts another quaternion from this one.
	 * @param rhs Quaternion to subtract.
	 */
	inline Quaternion operator-(const Quaternion & rhs) const
	{
		return Quaternion(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2], _v[3] - rhs._v[3]);
	}

	/**
	 * Subtracts another quaternion from this one.
	 * @param rhs Quaternion to subtract.
	 */
	inline Quaternion & operator-=(const Quaternion & rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;
	}

	/** Returns the negative of the quaternion. */
	inline Quaternion operator-() const
	{
		return Quaternion(-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	/** Returns length of the quaternion. */
	VALUE_TYPE length() const
	{
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
	}

	/** Returns length squared of the quaternion. */
	VALUE_TYPE length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
	}

	/** Returns conjugation of the quaternion. */
	inline Quaternion conj() const
	{
		return Quaternion(-_v[0], -_v[1], -_v[2], _v[3]);
	}

	/** Returns inverse of the quaternion. */
	inline Quaternion inverse() const
	{
		return conj() / length2();
	}

	/**
	 * Sets quaternion rotation from axis and angle.
	 * @param angle Rotation angle.
	 * @param x Rotation axis X component.
	 * @param y Rotation axis Y component.
	 * @param z Rotation axis z component.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle, VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z);

	/**
	 * Sets quaternion rotation from axis and angle.
	 * @param angle Rotation angle.
	 * @param axis Rotation axis.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle, const Vector3 & vec);

	/**
	 * Sets quaternion from three rotations.
	 * @param angle1 Rotation angle 1.
	 * @param axis1 Rotation axis 1.
	 * @param angle2 Rotation angle 2.
	 * @param axis2 Rotation axis 2.
	 * @param angle3 Rotation angle 3.
	 * @param axis3 Rotation axis 3.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle1, const Vector3 & axis1, 
		VALUE_TYPE angle2, const Vector3 & axis2,
		VALUE_TYPE angle3, const Vector3 & axis3);

	/**
	 * Makes a quaternion which will rotate from first vector to the second.
     * Generally takes a dot product to get the angle between these and then uses a
     * cross product to get the rotation axis. Watch out for the two special cases
     * when the vectors are co-incident or opposite in direction.
     */
	MATHAPI void makeRotate(const Vector3 & vec1, const Vector3 & vec2);

	/**
	 * Return the angle and vector components represented by the quaternion.
	 * @param angle Reference to the destination variable.
	 * @param x Reference to the destination variable.
	 * @param y Reference to the destination variable.
	 * @param z Reference to the destination variable.
	 */
	MATHAPI void getRotate(VALUE_TYPE & angle, VALUE_TYPE & x, VALUE_TYPE & y, VALUE_TYPE & z) const;

	/**
	 * Return the angle and vector represented by the quaternion.
	 * @param angle Reference to the destination variable.
	 * @param vec Reference to the destination variable.
	 */
	MATHAPI void getRotate(VALUE_TYPE & angle, Vector3 & vec) const;

	/**
	 * Spherical Linear Interpolation.
	 * As t goes from 0 to 1, the quaternion object goes from "from" to "to".
	 */
	MATHAPI void slerp(VALUE_TYPE t, const Quaternion & from, const Quaternion & to);

	/**
	 * Rotates a vector by this quaternion.
	 * @param v Vector to rotate.
	 */
	Vector3 operator*(const Vector3 & v) const
	{
		// NVidia SDK implementation
		Vector3 qvec(_v[0], _v[1], _v[2]);
		Vector3 uv = qvec ^ v;
		Vector3 uuv = qvec ^ uv; 
		uv *= (2.0f * _v[3]); 
		uuv *= 2.0f; 
		return v + uv + uuv;
	}

    /**
     * Rotates a vector by this quaternion.
     * @param v Vector to rotate.
     */
    Vector3 rotate(const Vector3 & v) const
    {
		Quaternion p(Vector4(v, 0.0f)), qConj(-_v[0], -_v[1], -_v[2], _v[3]);

		p = *this * p * qConj;
		return Vector3(p._v[0], p._v[1], p._v[2]);
    }

    /** Normalizes the quaternion. */
    Quaternion & normalize()
    {
		float m = length();

		if (m < 0.0001f)
		{
			stabilize_length();
			m = length();
		}

		float inv_length = 1.0f / m;
		_v[0] *= inv_length;
		_v[1] *= inv_length;
		_v[2] *= inv_length;

		return *this;
    }

    /** Stabilizes length of the quaternion. */
    void stabilize_length()
    {
		float cs = (float)(fabs(_v[0]) + fabs(_v[1]) + fabs(_v[2]) + fabs(_v[3]));

		if (cs > 0.0f)
		{
			_v[0] /= cs;
			_v[1] /= cs;
			_v[2] /= cs;
			_v[3] /= cs;
		}
		else
		{
			_v[0] = 0.0f;
			_v[1] = 0.0f;
			_v[2] = 0.0f;
			_v[3] = 1.0f;
		}
    }
};

Q_DECLARE_METATYPE(Quaternion);

#endif // __MATH__QUATERNION_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
