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
#ifndef __MATH__MATRIX_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__MATRIX_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>
#include <math/vector4.h>
#include <math/quaternion.h>


/* Classes */

/** Matrix class. */
class Matrix
{
public:
	/** Type of the component. */
	typedef float VALUE_TYPE;

	/** Default constructor. */
	inline Matrix()
	{
		makeIdentity();
	}

	/**
	 * Copy constructor.
	 * @param mat Matrix.
	 */
	inline Matrix(const Matrix & mat)
	{
		set(mat.ptr());
	}

	/**
	 * Constructor from array of floats.
	 * @param ptr Pointer to the array.
	 */
	inline explicit Matrix(float const * const ptr)
	{
		set(ptr);
	}

	/**
	 * Constructor from quaternion.
	 * @param quat Quaternion.
	 */
	inline explicit Matrix(const Quaternion & quat)
	{
		makeRotate(quat);
	}

	/**
	 * Constructor from values.
	 * @param a00 item at row 0 column 0.
	 * @param a01 item at row 0 column 1.
	 * @param a02 item at row 0 column 2.
	 * @param a03 item at row 0 column 3.
	 * @param a10 item at row 1 column 0.
	 * @param a11 item at row 1 column 1.
	 * @param a12 item at row 1 column 2.
	 * @param a13 item at row 1 column 3.
	 * @param a20 item at row 2 column 0.
	 * @param a21 item at row 2 column 1.
	 * @param a22 item at row 2 column 2.
	 * @param a23 item at row 2 column 3.
	 * @param a30 item at row 3 column 0.
	 * @param a31 item at row 3 column 1.
	 * @param a32 item at row 3 column 2.
	 * @param a33 item at row 3 column 3.
	 */
	MATHAPI Matrix(VALUE_TYPE a00, VALUE_TYPE a01, VALUE_TYPE a02, VALUE_TYPE a03,
		VALUE_TYPE a10, VALUE_TYPE a11, VALUE_TYPE a12, VALUE_TYPE a13,
		VALUE_TYPE a20, VALUE_TYPE a21, VALUE_TYPE a22, VALUE_TYPE a23,
		VALUE_TYPE a30, VALUE_TYPE a31, VALUE_TYPE a32, VALUE_TYPE a33);

	/**
	 * Compares this matrix with the specified one.
	 * @param m Matrix to compare with.
	 */
	MATHAPI int compare(const Matrix & m) const;

	/**
	 * Compares this matrix with the specified one.
	 * @param m Matrix to compare with.
	 */
	bool operator<(const Matrix & m) const
	{
		return compare(m) < 0;
	}

	/**
	 * Compares this matrix with the specified one.
	 * @param m Matrix to compare with.
	 */
	bool operator==(const Matrix & m) const
	{
		return compare(m) == 0;
	}

	/**
	 * Compares this matrix with the specified one.
	 * @param m Matrix to compare with.
	 */
	bool operator!=(const Matrix & m) const
	{
		return compare(m) != 0;
	}

	/**
	 * Returns the specified item of the matrix.
	 * @param row Row index.
	 * @param col Column index.
	 */
	inline VALUE_TYPE & operator()(int row, int col)
	{
		return _mat[row][col];
	}

	/**
	 * Returns the specified item of the matrix.
	 * @param row Row index.
	 * @param col Column index.
	 */
	inline VALUE_TYPE operator()(int row, int col) const
	{
		return _mat[row][col];
	}

	/** Returns true if all items of the matrix are valid numbers. */
	inline bool valid() const
	{
		return !isNaN();
	}

	/** Returns true if any of the matrix items is not a valid numbers. */
	inline bool isNaN() const
	{
		return ::isNaN(_mat[0][0]) || ::isNaN(_mat[0][1]) || ::isNaN(_mat[0][2]) || ::isNaN(_mat[0][3]) ||
			::isNaN(_mat[1][0]) || ::isNaN(_mat[1][1]) || ::isNaN(_mat[1][2]) || ::isNaN(_mat[1][3]) ||
			::isNaN(_mat[2][0]) || ::isNaN(_mat[2][1]) || ::isNaN(_mat[2][2]) || ::isNaN(_mat[2][3]) ||
			::isNaN(_mat[3][0]) || ::isNaN(_mat[3][1]) || ::isNaN(_mat[3][2]) || ::isNaN(_mat[3][3]);
	}

	/**
	 * Assignment operator.
	 * @param rhs Source matrix.
	 */
	inline Matrix & operator=(const Matrix & rhs)
	{
		if (&rhs == this)
			return *this;
		set(rhs.ptr());
		return *this;
	}

	/**
	 * Sets this matrix from the source matrix.
	 * @param rhs Source matrix.
	 */
	inline void set(const Matrix & rhs)
	{
		set(rhs.ptr());
	}

	/**
	 * Sets this matrix from array of floating point numbers.
	 * @param ptr Pointer to the array.
	 */
	inline void set(float const * const ptr)
	{
		VALUE_TYPE * local_ptr = (VALUE_TYPE *)_mat;
		for(int i = 0; i < 16; ++i)
			local_ptr[i] = (VALUE_TYPE)ptr[i];
	}

	/**
	 * Sets this matrix from values.
	 * @param a00 item at row 0 column 0.
	 * @param a01 item at row 0 column 1.
	 * @param a02 item at row 0 column 2.
	 * @param a03 item at row 0 column 3.
	 * @param a10 item at row 1 column 0.
	 * @param a11 item at row 1 column 1.
	 * @param a12 item at row 1 column 2.
	 * @param a13 item at row 1 column 3.
	 * @param a20 item at row 2 column 0.
	 * @param a21 item at row 2 column 1.
	 * @param a22 item at row 2 column 2.
	 * @param a23 item at row 2 column 3.
	 * @param a30 item at row 3 column 0.
	 * @param a31 item at row 3 column 1.
	 * @param a32 item at row 3 column 2.
	 * @param a33 item at row 3 column 3.
	 */
	MATHAPI void set(VALUE_TYPE a00, VALUE_TYPE a01, VALUE_TYPE a02,VALUE_TYPE a03,
		VALUE_TYPE a10, VALUE_TYPE a11, VALUE_TYPE a12,VALUE_TYPE a13,
		VALUE_TYPE a20, VALUE_TYPE a21, VALUE_TYPE a22,VALUE_TYPE a23,
		VALUE_TYPE a30, VALUE_TYPE a31, VALUE_TYPE a32,VALUE_TYPE a33);

	/** Returns pointer to the matrix items. */
	VALUE_TYPE * ptr()
	{
		return (VALUE_TYPE *)_mat;
	}

	/** Returns pointer to the matrix items. */
	const VALUE_TYPE * ptr() const
	{
		return (const VALUE_TYPE *)_mat;
	}

	/** Returns true if this is an identity matrix. */
	MATHAPI bool isIdentity() const;

	/** Makes this matrix an identity matrix. */
	MATHAPI void makeIdentity();

	/**
	 * Makes this matrix a scale matrix.
	 * @param v Scale vector.
	 */
	MATHAPI void makeScale(const Vector3 & v);

	/**
	 * Makes this matrix a scale matrix.
	 * @param x X scale.
	 * @param y Y scale.
	 * @param z Z scale.
	 */
	MATHAPI void makeScale(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z);

	/**
	 * Makes this matrix a translate matrix.
	 * @param v Translation vector.
	 */
	MATHAPI void makeTranslate(const Vector3 & v);

	/**
	 * Makes this matrix a translate matrix.
	 * @param x X translation.
	 * @param y Y translation.
	 * @param z Z translation.
	 */
	MATHAPI void makeTranslate(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z);

	/**
	 * Makes this matrix a rotation matrix.
	 * @param from Source vector.
	 * @param to Destination vector.
	 */
	MATHAPI void makeRotate(const Vector3 & from, const Vector3 & to);

	/**
	 * Makes this matrix a rotation matrix.
	 * @param angle Rotation angle.
	 * @param axis Rotation axis.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle, const Vector3 & axis);

	/**
	 * Makes this matrix a rotation matrix.
	 * @param angle Rotation angle.
	 * @param x X coordinate of the rotation axis.
	 * @param y Y coordinate of the rotation axis.
	 * @param z Z coordinate of the rotation axis.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle, VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z);

	/**
	 * Makes this matrix a rotation matrix.
	 * @param q Quaternion.
	 */
	MATHAPI void makeRotate(const Quaternion & q);

	/**
	 * Makes this matrix a rotation matrix.
	 * @param angle1 Rotation angle 1.
	 * @param axis1 Rotation axis 1.
	 * @param angle2 Rotation angle 2.
	 * @param axis2 Rotation axis 2.
	 * @param angle3 Rotation angle 3.
	 * @param axis3 Rotation axis 3.
	 */
	MATHAPI void makeRotate(VALUE_TYPE angle1, const Vector3 & axis1,
		VALUE_TYPE angle2, const Vector3 & axis2, VALUE_TYPE angle3, const Vector3 & axis3);

	/**
	 * Makes this matrix a projection matrix.
	 * @param left Left clipping plane.
	 * @param right Right clipping plane.
	 * @param bottom Bottom clipping plane.
	 * @param top Top clipping plane.
	 * @param zNear Near Z plane.
	 * @param zFar Far Z plane.
	 */
	MATHAPI void makeFrustum(VALUE_TYPE left, VALUE_TYPE right, VALUE_TYPE bottom, VALUE_TYPE top,
		VALUE_TYPE zNear, VALUE_TYPE zFar);

	/**
	 * Makes this matrix a projection matrix.
	 * @param fovy Vertical field of view (in degrees).
	 * @param aspect Aspect ratio of the viewport (width / height).
	 * @param zNear Near Z plane.
	 * @param zFar Far Z plane.
	 */
	MATHAPI void makePerspective(VALUE_TYPE fovy, VALUE_TYPE aspect, VALUE_TYPE zNear, VALUE_TYPE zFar);

	/**
	 * Sets this matrix to the inversion of the specified matrix.
	 * @param rhs Matrix to invert.
	 */
	inline bool invert(const Matrix & rhs)
	{
		bool is_4x3 = (rhs._mat[0][3] == 0.0f && rhs._mat[1][3] == 0.0f && rhs._mat[2][3] == 0.0f && rhs._mat[3][3] == 1.0f);
		return is_4x3 ? invert_4x3(rhs) : invert_4x4(rhs);
	}

	/**
	 * Sets this matrix to the 4x3 inversion of the specified matrix.
	 * Not right hand column is assumed to be 0,0,0,1.
	 * @param rhs Matrix to be inverted.
	 */
	MATHAPI bool invert_4x3(const Matrix & rhs);

	/**
	 * Sets this matrix to the 4x4 inversion of the specified matrix.
	 * @param rhs Matrix to be inverted.
	 */
	MATHAPI bool invert_4x4(const Matrix & rhs);

	/**
	 * Sets this matrix to the ortho-normalized 3x3 rotation & scale matrix.
	 * @param rhs Matrix to be ortho-normalized.
	 */ 
	MATHAPI void orthoNormalize(const Matrix & rhs); 

	/** Returns identity matrix. */
	inline static Matrix identity()
	{
		Matrix m;
		m.makeIdentity();
		return m;
	}

	/**
	 * Returns scale matrix.
	 * @param sx X scale.
	 * @param sy Y scale.
	 * @param sz Z scale.
	 */
	inline static Matrix scale(VALUE_TYPE sx, VALUE_TYPE sy, VALUE_TYPE sz)
	{
		Matrix m;
		m.makeScale(sx, sy, sz);
		return m;
	}

	/**
	 * Returns scale matrix.
	 * @param sv Scale vector.
	 */
	inline static Matrix scale(const Vector3 & sv)
	{
		Matrix m;
		m.makeScale(sv);
		return m;
	}

	/**
	 * Returns translation matrix.
	 * @param x X translation.
	 * @param y Y translation.
	 * @param z Z translation.
	 */
	inline static Matrix translate(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
	{
		Matrix m;
		m.makeTranslate(x, y, z);
		return m;
	}

	/**
	 * Returns translation matrix.
	 * @param dv Translation vector.
	 */
	inline static Matrix translate(const Vector3 & dv)
	{
		Matrix m;
		m.makeTranslate(dv);
		return m;
	}

	/**
	 * Returns rotation matrix.
	 * @param from Source vector.
	 * @param to Destination vector.
	 */
	inline static Matrix rotate(const Vector3 & from, const Vector3 & to)
	{
		Matrix m;
		m.makeRotate(from, to);
		return m;
	}

	/**
	 * Returns rotation matrix.
	 * @param angle Rotation angle.
	 * @param x X coordinate of the rotation axis.
	 * @param y Y coordinate of the rotation axis.
	 * @param z Z coordinate of the rotation axis.
	 */
	inline static Matrix rotate(VALUE_TYPE angle, VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
	{
		Matrix m;
		m.makeRotate(angle, x, y, z);
		return m;
	}

	/**
	 * Returns rotation matrix.
	 * @param angle Rotation angle.
	 * @param axis Rotation axis.
	 */
	inline static Matrix rotate(VALUE_TYPE angle, const Vector3 & axis)
	{
		Matrix m;
		m.makeRotate(angle, axis);
		return m;
	}

	/**
	 * Returns rotation matrix.
	 * @param angle1 Rotation angle 1.
	 * @param axis1 Rotation axis 1.
	 * @param angle2 Rotation angle 2.
	 * @param axis2 Rotation axis 2.
	 * @param angle3 Rotation angle 3.
	 * @param axis3 Rotation axis 3.
	 */
	inline static Matrix rotate(VALUE_TYPE angle1, const Vector3 & axis1,
		VALUE_TYPE angle2, const Vector3 & axis2, VALUE_TYPE angle3, const Vector3 & axis3)
	{
		Matrix m;
		m.makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
		return m;
	}

	/**
	 * Returns rotation matrix.
	 * @param quat Quaternion.
	 */
	inline static Matrix rotate(const Quaternion & quat)
	{
		return Matrix(quat);
	}

	/**
	 * Returns inverse matrix.
	 * @param m Source matrix.
	 */
	inline static Matrix inverse(const Matrix & m)
	{
		Matrix m1;
		m1.invert(m);
		return m1;
	}

	/**
	 * Returns ortho-normalized matrix.
	 * @param m Source matrix.
	 */
	inline static Matrix orthoNormal(const Matrix & m)
	{
		Matrix m1;
		m1.orthoNormalize(m);
		return m1;
	}

	/**
	 * Multiplies vector by this matrix.
	 * @param v Vector to multiply.
	 */
	inline Vector3 preMult(const Vector3 & v) const
	{
		VALUE_TYPE d = 1.0f / (_mat[0][3] * v._v[0] + _mat[1][3] * v._v[1] + _mat[2][3] * v._v[2] + _mat[3][3]);
		return Vector3
		(
			(_mat[0][0] * v._v[0] + _mat[1][0] * v._v[1] + _mat[2][0] * v._v[2] + _mat[3][0]) * d,
			(_mat[0][1] * v._v[0] + _mat[1][1] * v._v[1] + _mat[2][1] * v._v[2] + _mat[3][1]) * d,
			(_mat[0][2] * v._v[0] + _mat[1][2] * v._v[1] + _mat[2][2] * v._v[2] + _mat[3][2]) * d
		);
	}

	/**
	 * Multiplies this matrix by the vector.
	 * @param v Vector to multiply.
	 */
	inline Vector3 postMult(const Vector3 & v) const
	{
		VALUE_TYPE d = 1.0f / (_mat[3][0] * v._v[0] + _mat[3][1] * v._v[1] + _mat[3][2] * v._v[2] + _mat[3][3]);
		return Vector3
		(
			(_mat[0][0] * v._v[0] + _mat[0][1] * v._v[1] + _mat[0][2] * v._v[2] + _mat[0][3]) * d,
			(_mat[1][0] * v._v[0] + _mat[1][1] * v._v[1] + _mat[1][2] * v._v[2] + _mat[1][3]) * d,
			(_mat[2][0] * v._v[0] + _mat[2][1] * v._v[1] + _mat[2][2] * v._v[2] + _mat[2][3]) * d
		);
	}

	/**
	 * Multiplies this matrix by the vector.
	 * @param v Vector to multiply.
	 */
	inline Vector3 operator*(const Vector3 & v) const
	{
		return postMult(v);
	}

	/**
	 * Multiplies vector by this matrix.
	 * @param v Vector to multiply.
	 */
	inline Vector4 preMult(const Vector4 & v) const
	{
		return Vector4
		(
			(_mat[0][0] * v._v[0] + _mat[1][0] * v._v[1] + _mat[2][0] * v._v[2] + _mat[3][0] * v._v[3]),
			(_mat[0][1] * v._v[0] + _mat[1][1] * v._v[1] + _mat[2][1] * v._v[2] + _mat[3][1] * v._v[3]),
			(_mat[0][2] * v._v[0] + _mat[1][2] * v._v[1] + _mat[2][2] * v._v[2] + _mat[3][2] * v._v[3]),
			(_mat[0][3] * v._v[0] + _mat[1][3] * v._v[1] + _mat[2][3] * v._v[2] + _mat[3][3] * v._v[3])
		);
	}

	/**
	 * Multiplies this matrix by the vector.
	 * @param v Vector to multiply.
	 */
	inline Vector4 postMult(const Vector4 & v) const
	{
		return Vector4
		(
			(_mat[0][0] * v._v[0] + _mat[0][1] * v._v[1] + _mat[0][2] * v._v[2] + _mat[0][3] * v._v[3]),
			(_mat[1][0] * v._v[0] + _mat[1][1] * v._v[1] + _mat[1][2] * v._v[2] + _mat[1][3] * v._v[3]),
			(_mat[2][0] * v._v[0] + _mat[2][1] * v._v[1] + _mat[2][2] * v._v[2] + _mat[2][3] * v._v[3]),
			(_mat[3][0] * v._v[0] + _mat[3][1] * v._v[1] + _mat[3][2] * v._v[2] + _mat[3][3] * v._v[3])
		);
	}

	/**
	 * Multiplies this matrix by the vector.
	 * @param v Vector to multiply.
	 */
	inline Vector4 operator*(const Vector4 & v) const
	{
		return postMult(v);
	}

	/**
	 * Sets rotation component of the matrix.
	 * @param q Source quaternion.
	 */
	MATHAPI void setRotate(const Quaternion & q);

	/**
	 * Translates the matrix rotation component into the quaternion.
	 * Note that this function assumes a non-scaled matrix and will return incorrect results
	 * for scaled matrices.
	 */
	MATHAPI Quaternion getRotate() const;

	/**
	 * Sets translation component of the matrix.
	 * @param tx X coordinate.
	 * @param ty Y coordinate.
	 * @param tz Z coordinate.
	 */
	MATHAPI void setTrans(VALUE_TYPE tx, VALUE_TYPE ty, VALUE_TYPE tz);

	/**
	 * Sets translation component of the matrix.
	 * @param v Translation vector.
	 */
	MATHAPI void setTrans(const Vector3 & v);

	/** Returns translation component of the matrix. */
	inline Vector3 getTrans() const
	{
		return Vector3(_mat[3][0], _mat[3][1], _mat[3][2]);
	} 

	/** Returns scale component of the matrix. */
	inline Vector3 getScale() const
	{
		Vector3 x_vec(_mat[0][0], _mat[1][0], _mat[2][0]); 
		Vector3 y_vec(_mat[0][1], _mat[1][1], _mat[2][1]); 
		Vector3 z_vec(_mat[0][2], _mat[1][2], _mat[2][2]); 

		return Vector3(x_vec.length(), y_vec.length(), z_vec.length());
	}

	/**
	 * Sets this matrix to the result of matrix multiplication.
	 * @param m1 First matrix.
	 * @param m2 Second matrix.
	 */
	MATHAPI void mult(const Matrix & m1, const Matrix & m2);

	/**
	 * Premultiplies this matrix by the specified matrix.
	 * @param other Second matrix.
	 */
	MATHAPI void preMult(const Matrix & other);

	/**
	 * Postmultiplies this matrix by the specified matrix.
	 * @param other Second matrix.
	 */
	MATHAPI void postMult(const Matrix & other);

	/**
	 * Applies a 3x3 transform of v * M[0..2,0..2].
	 * @param v Vector.
	 * @param m Matrix.
	 */
	inline static Vector3 transform3x3(const Vector3 & v, const Matrix & m)
	{
		return Vector3
		(
			(m._mat[0][0] * v._v[0] + m._mat[1][0] * v._v[1] + m._mat[2][0] * v._v[2]),
			(m._mat[0][1] * v._v[0] + m._mat[1][1] * v._v[1] + m._mat[2][1] * v._v[2]),
			(m._mat[0][2] * v._v[0] + m._mat[1][2] * v._v[1] + m._mat[2][2] * v._v[2])
		);
	}

	/**
	 * Applies a 3x3 transform of v * M[0..2,0..2].
	 * @param m Matrix.
	 * @param v Vector.
	 */
	inline static Vector3 transform3x3(const Matrix & m, const Vector3 & v)
	{
		return Vector3
		(
			(m._mat[0][0] * v._v[0] + m._mat[0][1] * v._v[1] + m._mat[0][2] * v._v[2]),
			(m._mat[1][0] * v._v[0] + m._mat[1][1] * v._v[1] + m._mat[1][2] * v._v[2]),
			(m._mat[2][0] * v._v[0] + m._mat[2][1] * v._v[1] + m._mat[2][2] * v._v[2])
		);
	}

	/**
	 * Optimized version of preMult(translate(v));
	 * @param v Vector.
	 */
	inline void preMultTranslate(const Vector3 & v)
	{
		for (unsigned i = 0; i < 3; ++i)
		{
			float tmp = v[i];
			if (tmp == 0.0f)
				continue;

			_mat[3][0] += tmp * _mat[i][0];
			_mat[3][1] += tmp * _mat[i][1];
			_mat[3][2] += tmp * _mat[i][2];
			_mat[3][3] += tmp * _mat[i][3];
		}
	}

	/**
	 * Optimized version of postMult(translate(v)); 
	 * @param v Vector.
	 */
	inline void postMultTranslate(const Vector3 & v)
	{
		for (unsigned i = 0; i < 3; ++i)
		{
			float tmp = v[i];
			if (tmp == 0.0f)
				continue;
			_mat[0][i] += tmp * _mat[0][3];
			_mat[1][i] += tmp * _mat[1][3];
			_mat[2][i] += tmp * _mat[2][3];
			_mat[3][i] += tmp * _mat[3][3];
		}
	}

	/**
	 * Optimized version of preMult(scale(v));
	 * @param v Vector.
	 */
	inline void preMultScale(const Vector3 & v)
	{
		_mat[0][0] *= v[0];
		_mat[0][1] *= v[0];
		_mat[0][2] *= v[0];
		_mat[0][3] *= v[0];

		_mat[1][0] *= v[1];
		_mat[1][1] *= v[1];
		_mat[1][2] *= v[1];
		_mat[1][3] *= v[1];

		_mat[2][0] *= v[2];
		_mat[2][1] *= v[2];
		_mat[2][2] *= v[2];
		_mat[2][3] *= v[2];
	}

	/**
	 * Optimized version of postMult(scale(v));
	 * @param v Vector.
	 */
	inline void postMultScale(const Vector3 & v)
	{
		_mat[0][0] *= v[0];
		_mat[1][0] *= v[0];
		_mat[2][0] *= v[0];
		_mat[3][0] *= v[0];

		_mat[0][1] *= v[1];
		_mat[1][1] *= v[1];
		_mat[2][1] *= v[1];
		_mat[3][1] *= v[1];

		_mat[0][2] *= v[2];
		_mat[1][2] *= v[2];
		_mat[2][2] *= v[2];
		_mat[3][2] *= v[2];
	}

	/**
	 * Optimized version of preMult(rotate(q));
	 * @param v Vector.
	 */
	inline void preMultRotate(const Quaternion & q)
	{
		if (q.zeroRotation())
			return;

		Matrix r;
		r.setRotate(q);
		preMult(r);
	}

	/**
	 * Optimized version of postMult(rotate(q));
	 * @param v Vector.
	 */
	inline void postMultRotate(const Quaternion & q)
	{
		if (q.zeroRotation())
			return;

		Matrix r;
		r.setRotate(q);
		postMult(r);
	}

	/**
	 * Multiplies this matrix on another one.
	 * @param other Matrix to multiply this on.
	 */
	inline void operator*=(const Matrix & other) 
	{
		if (this != &other)
			postMult(other);
		else
		{
			Matrix temp(other);
			postMult(temp);
		}
	}

	/**
	 * Multiplies this matrix on another one.
	 * @param m Matrix to multiply this on.
	 */
	inline Matrix operator*(const Matrix & m) const
	{
		Matrix r;
		r.mult(*this, m);
		return r;
	}

protected:
	VALUE_TYPE _mat[4][4];		/**< Matrix data. */
};

Q_DECLARE_METATYPE(Matrix)


/* Functions */

/**
 * Multiplies vector by the matrix.
 * @param v Vector.
 * @param m Matrix.
 */
inline Vector3 operator*(const Vector3 & v, const Matrix & m)
{
	return m.preMult(v);
}

/**
 * Multiplies vector by the matrix.
 * @param v Vector.
 * @param m Matrix.
 */
inline Vector4 operator*(const Vector4 & v, const Matrix & m)
{
	return m.preMult(v);
}

#endif // __MATH__MATRIX_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
