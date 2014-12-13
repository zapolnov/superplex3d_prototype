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
#include <math/matrix.h>
#include <limits>

/* Macros */

#define SET_ROW(row, v1, v2, v3, v4) \
	_mat[(row)][0] = (v1); \
	_mat[(row)][1] = (v2); \
	_mat[(row)][2] = (v3); \
	_mat[(row)][3] = (v4);

#define INNER_PRODUCT(a, b, r, c) \
	  ((a)._mat[r][0] * (b)._mat[0][c]) \
	+ ((a)._mat[r][1] * (b)._mat[1][c]) \
	+ ((a)._mat[r][2] * (b)._mat[2][c]) \
	+ ((a)._mat[r][3] * (b)._mat[3][c])

#define QX (q._v[0])
#define QY (q._v[1])
#define QZ (q._v[2])
#define QW (q._v[3])

#ifndef SGL_SWAP
 #define SGL_SWAP(a, b, temp) ((temp) = (a), (a) = (b), (b) = (temp))
#endif


/* Private functions */

//
// Returns absolute value of the specified value
//
template <class T>
inline T SGL_ABS(T a)
{
	return (a >= 0 ? a : -a);
}


/* Matrix methods */

//
// Constructor
//
Matrix::Matrix(VALUE_TYPE a00, VALUE_TYPE a01, VALUE_TYPE a02, VALUE_TYPE a03,
	VALUE_TYPE a10, VALUE_TYPE a11, VALUE_TYPE a12, VALUE_TYPE a13,
	VALUE_TYPE a20, VALUE_TYPE a21, VALUE_TYPE a22, VALUE_TYPE a23,
	VALUE_TYPE a30, VALUE_TYPE a31, VALUE_TYPE a32, VALUE_TYPE a33)
{
	SET_ROW(0, a00, a01, a02, a03);
	SET_ROW(1, a10, a11, a12, a13);
	SET_ROW(2, a20, a21, a22, a23);
	SET_ROW(3, a30, a31, a32, a33);
}

//
// Compares this matrix with the specified one
//
int Matrix::compare(const Matrix & m) const
{
	const VALUE_TYPE * lhs = reinterpret_cast<const VALUE_TYPE *>(_mat);
	const VALUE_TYPE * end_lhs = lhs + 16;
	const VALUE_TYPE * rhs = reinterpret_cast<const VALUE_TYPE *>(m._mat);
	for(; lhs != end_lhs; ++lhs, ++rhs)
	{
		if (*lhs < *rhs)
			return -1;
		if (*rhs < *lhs)
			return 1;
    }

	return 0;
}

//
// Sets this matrix from values
//
void Matrix::set(VALUE_TYPE a00, VALUE_TYPE a01, VALUE_TYPE a02, VALUE_TYPE a03,
	VALUE_TYPE a10, VALUE_TYPE a11, VALUE_TYPE a12, VALUE_TYPE a13,
	VALUE_TYPE a20, VALUE_TYPE a21, VALUE_TYPE a22, VALUE_TYPE a23,
	VALUE_TYPE a30, VALUE_TYPE a31, VALUE_TYPE a32, VALUE_TYPE a33)
{
	SET_ROW(0, a00, a01, a02, a03);
	SET_ROW(1, a10, a11, a12, a13);
	SET_ROW(2, a20, a21, a22, a23);
	SET_ROW(3, a30, a31, a32, a33);
}

//
// Returns true if this is an identity matrix
//
bool Matrix::isIdentity() const
{
	return _mat[0][0] == 1.0f && _mat[0][1] == 0.0f && _mat[0][2] == 0.0f &&  _mat[0][3] == 0.0f &&
		   _mat[1][0] == 0.0f && _mat[1][1] == 1.0f && _mat[1][2] == 0.0f &&  _mat[1][3] == 0.0f &&
		   _mat[2][0] == 0.0f && _mat[2][1] == 0.0f && _mat[2][2] == 1.0f &&  _mat[2][3] == 0.0f &&
		   _mat[3][0] == 0.0f && _mat[3][1] == 0.0f && _mat[3][2] == 0.0f &&  _mat[3][3] == 1.0f;
}

//
// Makes this matrix an identity matrix
//
void Matrix::makeIdentity()
{
	SET_ROW(0, 1.0, 0.0, 0.0, 0.0);
	SET_ROW(1, 0.0, 1.0, 0.0, 0.0);
	SET_ROW(2, 0.0, 0.0, 1.0, 0.0);
	SET_ROW(3, 0.0, 0.0, 0.0, 1.0);
}

//
// Makes this matrix a scale matrix
//
void Matrix::makeScale(const Vector3 & v)
{
	makeScale(v._v[0], v._v[1], v._v[2]);
}

//
// Makes this matrix a scale matrix
//
void Matrix::makeScale(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
{
	SET_ROW(0, x, 0, 0, 0);
	SET_ROW(1, 0, y, 0, 0);
	SET_ROW(2, 0, 0, z, 0);
	SET_ROW(3, 0, 0, 0, 1);
}

//
// Makes this matrix a translate matrix
//
void Matrix::makeTranslate(const Vector3 & v)
{
	makeTranslate(v._v[0], v._v[1], v._v[2]);
}

//
// Makes this matrix a translate matrix
//
void Matrix::makeTranslate(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
{
	SET_ROW(0, 1, 0, 0, 0);
	SET_ROW(1, 0, 1, 0, 0);
	SET_ROW(2, 0, 0, 1, 0);
	SET_ROW(3, x, y, z, 1);
}

//
// Makes this matrix a rotation matrix
//
void Matrix::makeRotate(const Vector3 & from, const Vector3 & to)
{
	makeIdentity();

	Quaternion q;
	q.makeRotate(from, to);

	setRotate(q);
}

//
// Makes this matrix a rotation matrix
//
void Matrix::makeRotate(VALUE_TYPE angle, const Vector3 & axis)
{
	makeIdentity();

	Quaternion q;
	q.makeRotate(angle, axis);

    setRotate(q);
}

//
// Makes this matrix a rotation matrix
//
void Matrix::makeRotate(VALUE_TYPE angle, VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z) 
{
	makeIdentity();

	Quaternion q;
	q.makeRotate(angle, x, y, z);

	setRotate(q);
}

//
// Makes this matrix a rotation matrix
//
void Matrix::makeRotate(const Quaternion & q)
{
	makeIdentity();
	setRotate(q);
}

//
// Makes this matrix a rotation matrix
//
void Matrix::makeRotate(VALUE_TYPE angle1, const Vector3 & axis1, 
	VALUE_TYPE angle2, const Vector3 & axis2, VALUE_TYPE angle3, const Vector3 & axis3)
{
	makeIdentity();

	Quaternion q;
	q.makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	setRotate(q);
}

//
// Makes this matrix a projection matrix
//
void Matrix::makeFrustum(VALUE_TYPE left, VALUE_TYPE right, VALUE_TYPE bottom, VALUE_TYPE top,
	VALUE_TYPE zNear, VALUE_TYPE zFar)
{
	VALUE_TYPE A = (right + left) / (right - left);
	VALUE_TYPE B = (top + bottom) / (top - bottom);
	VALUE_TYPE C = -(zFar + zNear) / (zFar - zNear);
	VALUE_TYPE D = -2.0f * zFar * zNear / (zFar - zNear);

	SET_ROW(0, 2.0f * zNear / (right - left), 0.0f, 0.0f, 0.0f)
	SET_ROW(1, 0.0f, 2.0f * zNear / (top - bottom), 0.0f, 0.0f)
	SET_ROW(2, A, B, C, -1.0f)
	SET_ROW(3, 0.0f, 0.0f, D, 0.0f)
}

//
// Makes this matrix a projection matrix
//
void Matrix::makePerspective(VALUE_TYPE fovy, VALUE_TYPE aspect, VALUE_TYPE zNear, VALUE_TYPE zFar)
{
	VALUE_TYPE tan_fovy = tan(degreesToRadians(fovy * 0.5f));
	VALUE_TYPE right = tan_fovy * aspect * zNear;
	VALUE_TYPE left = -right;
	VALUE_TYPE top = tan_fovy * zNear;
	VALUE_TYPE bottom = -top;
	makeFrustum(left, right, bottom, top, zNear, zFar);
}

//
// Sets this matrix to the 4x3 inversion of the specified matrix
//
bool Matrix::invert_4x3(const Matrix & mat)
{
	if (&mat == this)
	{
		Matrix tm(mat);
		return invert_4x3(tm);
	}

	// Copy rotation components directly into registers for speed
	register VALUE_TYPE r00 = mat._mat[0][0];
	register VALUE_TYPE r01 = mat._mat[0][1];
	register VALUE_TYPE r02 = mat._mat[0][2];
	register VALUE_TYPE r10 = mat._mat[1][0];
	register VALUE_TYPE r11 = mat._mat[1][1];
	register VALUE_TYPE r12 = mat._mat[1][2];
	register VALUE_TYPE r20 = mat._mat[2][0];
	register VALUE_TYPE r21 = mat._mat[2][1];
	register VALUE_TYPE r22 = mat._mat[2][2];

	// Partially compute inverse of rot
	_mat[0][0] = r11 * r22 - r12 * r21;
    _mat[0][1] = r02 * r21 - r01 * r22;
    _mat[0][2] = r01 * r12 - r02 * r11;

	// Compute determinant of rot from 3 elements just computed
	register VALUE_TYPE one_over_det = 1.0f / (r00 * _mat[0][0] + r10 * _mat[0][1] + r20 * _mat[0][2]);
	r00 *= one_over_det;
	r10 *= one_over_det;
	r20 *= one_over_det;

	// Finish computing inverse of rot
	_mat[0][0] *= one_over_det;
	_mat[0][1] *= one_over_det;
	_mat[0][2] *= one_over_det;
	_mat[0][3] = 0.0;
	_mat[1][0] = r12 * r20 - r10 * r22;	// Have already been divided by det
	_mat[1][1] = r00 * r22 - r02 * r20;	// same
	_mat[1][2] = r02 * r10 - r00 * r12;	// same
	_mat[1][3] = 0.0;
	_mat[2][0] = r10 * r21 - r11 * r20;	// Have already been divided by det
	_mat[2][1] = r01 * r20 - r00 * r21;	// same
	_mat[2][2] = r00 * r11 - r01 * r10;	// same
	_mat[2][3] = 0.0;
	_mat[3][3] = 1.0;

	#define d r22
	#define tx r10
	#define ty r11
	#define tz r12

	// We no longer need the rxx or det variables anymore, so we can reuse them for
	// whatever we want.  But we will still rename them for the sake of clarity.
	d = mat._mat[3][3];
	if (square(d - 1.0) > 1.0e-6)
    {
		Matrix TPinv;

		// Involves perspective, so we must compute the full inverse
		_mat[3][0] = 0.0;
		_mat[3][1] = 0.0;
		_mat[3][2] = 0.0;

		#define px r00
		#define py r01
		#define pz r02
		#define one_over_s one_over_det

		#define a r10
		#define b r11
		#define c r12

		a = mat._mat[0][3];
		b = mat._mat[1][3];
		c = mat._mat[2][3];
		px = _mat[0][0] * a + _mat[0][1] * b + _mat[0][2] * c;
		py = _mat[1][0] * a + _mat[1][1] * b + _mat[1][2] * c;
		pz = _mat[2][0] * a + _mat[2][1] * b + _mat[2][2] * c;

		#undef c
		#undef b
		#undef a

		tx = mat._mat[3][0];
		ty = mat._mat[3][1];
		tz = mat._mat[3][2];
		one_over_s = 1.0f / (d - (tx * px + ty * py + tz * pz));

		tx *= one_over_s;
		ty *= one_over_s;
		tz *= one_over_s;

		// Reduces number of calculations later on
        // Compute inverse of trans * corr
		TPinv._mat[0][0] = tx * px + 1.0f;
		TPinv._mat[0][1] = ty * px;
		TPinv._mat[0][2] = tz * px;
		TPinv._mat[0][3] = -px * one_over_s;
		TPinv._mat[1][0] = tx * py;
		TPinv._mat[1][1] = ty * py + 1.0f;
		TPinv._mat[1][2] = tz * py;
		TPinv._mat[1][3] = -py * one_over_s;
		TPinv._mat[2][0] = tx * pz;
		TPinv._mat[2][1] = ty * pz;
		TPinv._mat[2][2] = tz * pz + 1.0f;
		TPinv._mat[2][3] = -pz * one_over_s;
		TPinv._mat[3][0] = -tx;
		TPinv._mat[3][1] = -ty;
		TPinv._mat[3][2] = -tz;
		TPinv._mat[3][3] = one_over_s;

		// Finish computing full inverse of mat
		preMult(TPinv);

		#undef one_over_s
		#undef pz
		#undef py
		#undef px
	}
	else
    {
    	// Rightmost column is [0; 0; 0; 1] so it can be ignored
		tx = mat._mat[3][0];
		ty = mat._mat[3][1];
		tz = mat._mat[3][2];

		// Compute translation components of mat'
		_mat[3][0] = -(tx * _mat[0][0] + ty * _mat[1][0] + tz * _mat[2][0]);
		_mat[3][1] = -(tx * _mat[0][1] + ty * _mat[1][1] + tz * _mat[2][1]);
		_mat[3][2] = -(tx * _mat[0][2] + ty * _mat[1][2] + tz * _mat[2][2]);

    }

	#undef tz
	#undef ty
	#undef tx
	#undef d

    return true;
}

//
// Sets this matrix to the 4x4 inversion of the specified matrix
//
bool Matrix::invert_4x4(const Matrix & rhs)
{
	if (&rhs == this)
	{
		Matrix tm(rhs);
		return invert_4x4(tm);
	}

	unsigned int indxc[4], indxr[4], ipiv[4];
	unsigned int i, j, k, l, ll;
	unsigned int icol = 0;
	unsigned int irow = 0;
	double pivinv, dum, big;
	VALUE_TYPE temp;

	// copy in place this may be unnecessary
    *this = rhs;

	for (j = 0; j < 4; j++)
		ipiv[j] = 0;
    for (i = 0; i < 4; i++)
    {
		big = 0.0;
		for (j = 0; j < 4; j++)
		{
			if (ipiv[j] != 1)
			{
				for (k = 0; k < 4; k++)
				{
					if (ipiv[k] == 0)
					{
						if (SGL_ABS(operator()(j, k)) >= big)
						{
							big = SGL_ABS(operator()(j, k));
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
						return false;
				}
			}
		}

		++(ipiv[icol]);
		if (irow != icol)
		{
			for (l = 0; l < 4; l++)
				SGL_SWAP(operator()(irow, l), operator()(icol, l), temp);
		}

		indxr[i] = irow;
		indxc[i] = icol;
		if (operator()(icol, icol) == 0)
			return false;

		pivinv = 1.0 / operator()(icol, icol);
		operator()(icol,icol) = 1;
		for (l = 0; l < 4; l++)
			operator()(icol, l) *= (float)pivinv;
		for (ll = 0; ll < 4; ll++)
		{
			if (ll != icol)
			{
				dum = operator()(ll, icol);
				operator()(ll, icol) = 0;
				for (l = 0; l < 4; l++)
					operator()(ll, l) -= operator()(icol, l) * (float)dum;
			}
		}
	}

	for (int lx = 4; lx > 0; --lx)
	{
		if (indxr[lx - 1] != indxc[lx - 1])
		{
			for (k = 0; k < 4; k++)
				SGL_SWAP(operator()(k, indxr[lx - 1]), operator()(k, indxc[lx - 1]), temp);
		}
    }

    return true;
}

//
// Sets this matrix to the ortho-normalized 3x3 rotation & scale matrix
//
void Matrix::orthoNormalize(const Matrix & rhs)
{
	VALUE_TYPE x_colMag = (rhs._mat[0][0] * rhs._mat[0][0]) +
		(rhs._mat[1][0] * rhs._mat[1][0]) + (rhs._mat[2][0] * rhs._mat[2][0]);
	VALUE_TYPE y_colMag = (rhs._mat[0][1] * rhs._mat[0][1]) +
		(rhs._mat[1][1] * rhs._mat[1][1]) + (rhs._mat[2][1] * rhs._mat[2][1]);
	VALUE_TYPE z_colMag = (rhs._mat[0][2] * rhs._mat[0][2]) +
		(rhs._mat[1][2] * rhs._mat[1][2]) + (rhs._mat[2][2] * rhs._mat[2][2]);

	if (!equivalent((double)x_colMag, 1.0) && !equivalent((double)x_colMag, 0.0))
	{
		x_colMag = sqrt(x_colMag);
		_mat[0][0] = rhs._mat[0][0] / x_colMag;
		_mat[1][0] = rhs._mat[1][0] / x_colMag;
		_mat[2][0] = rhs._mat[2][0] / x_colMag;
	}
	else
	{
		_mat[0][0] = rhs._mat[0][0];
		_mat[1][0] = rhs._mat[1][0];
		_mat[2][0] = rhs._mat[2][0];
	}

	if (!equivalent((double)y_colMag, 1.0) && !equivalent((double)y_colMag, 0.0))
	{
		y_colMag = sqrt(y_colMag);
		_mat[0][1] = rhs._mat[0][1] / y_colMag;
		_mat[1][1] = rhs._mat[1][1] / y_colMag;
		_mat[2][1] = rhs._mat[2][1] / y_colMag;
	}
	else
	{
		_mat[0][1] = rhs._mat[0][1];
		_mat[1][1] = rhs._mat[1][1];
		_mat[2][1] = rhs._mat[2][1];
	}

	if (!equivalent((double)z_colMag, 1.0) && !equivalent((double)z_colMag, 0.0))
	{
		z_colMag = sqrt(z_colMag);
		_mat[0][2] = rhs._mat[0][2] / z_colMag;
		_mat[1][2] = rhs._mat[1][2] / z_colMag;
		_mat[2][2] = rhs._mat[2][2] / z_colMag;
	}
	else
	{
		_mat[0][2] = rhs._mat[0][2];
		_mat[1][2] = rhs._mat[1][2];
		_mat[2][2] = rhs._mat[2][2];
	}

	_mat[3][0] = rhs._mat[3][0];
	_mat[3][1] = rhs._mat[3][1];
	_mat[3][2] = rhs._mat[3][2];

	_mat[0][3] = rhs._mat[0][3];
	_mat[1][3] = rhs._mat[1][3];
	_mat[2][3] = rhs._mat[2][3];
	_mat[3][3] = rhs._mat[3][3];
}

//
// Sets rotation component of the matrix
//
void Matrix::setRotate(const Quaternion & q)
{
	double length2 = q.length2();
	double rlength2;

	// normalize quat if required. We can avoid the expensive sqrt in this case since
	// all 'coefficients' below are products of two q components. That is a square of
	// a square root, so it is possible to avoid that
	if (length2 != 1.0)
		rlength2 = 2.0 / length2;
	else
		rlength2 = 2.0;

	// calculate coefficients
	double x2 = rlength2 * QX;
	double y2 = rlength2 * QY;
	double z2 = rlength2 * QZ;
	double xx = QX * x2;
	double xy = QX * y2;
	double xz = QX * z2;
	double yy = QY * y2;
	double yz = QY * z2;
	double zz = QZ * z2;
	double wx = QW * x2;
	double wy = QW * y2;
	double wz = QW * z2;

	_mat[0][0] = (VALUE_TYPE)(1.0 - (yy + zz));
	_mat[1][0] = (VALUE_TYPE)(xy - wz);
	_mat[2][0] = (VALUE_TYPE)(xz + wy);
	_mat[0][1] = (VALUE_TYPE)(xy + wz);
	_mat[1][1] = (VALUE_TYPE)(1.0f - (xx + zz));
	_mat[2][1] = (VALUE_TYPE)(yz - wx);
	_mat[0][2] = (VALUE_TYPE)(xz - wy);
	_mat[1][2] = (VALUE_TYPE)(yz + wx);
	_mat[2][2] = (VALUE_TYPE)(1.0f - (xx + yy));
}

//
// Translates the matrix rotation component into the quaternion
//
Quaternion Matrix::getRotate() const
{
	VALUE_TYPE s, tq[4];
	Quaternion q;
	int i, j;

	// Use tq to store the largest trace
	tq[0] = 1 + _mat[0][0] + _mat[1][1] + _mat[2][2];
	tq[1] = 1 + _mat[0][0] - _mat[1][1] - _mat[2][2];
	tq[2] = 1 - _mat[0][0] + _mat[1][1] - _mat[2][2];
	tq[3] = 1 - _mat[0][0] - _mat[1][1] + _mat[2][2];

	// Find the maximum (could also use stacked if's later)
	j = 0;
	for (i = 1; i < 4; i++)
		j = tq[i] > tq[j] ? i : j;

	// Check the diagonal
	if (j == 0)
	{
		// perform instant calculation
		QW = tq[0];
		QX = _mat[1][2] - _mat[2][1]; 
		QY = _mat[2][0] - _mat[0][2]; 
		QZ = _mat[0][1] - _mat[1][0]; 
	}
	else if (j == 1)
	{
		QW = _mat[1][2] - _mat[2][1]; 
		QX = tq[1];
		QY = _mat[0][1] + _mat[1][0]; 
		QZ = _mat[2][0] + _mat[0][2]; 
	}
	else if (j == 2)
	{
		QW = _mat[2][0] - _mat[0][2]; 
		QX = _mat[0][1] + _mat[1][0]; 
		QY = tq[2];
		QZ = _mat[1][2] + _mat[2][1]; 
	}
	else
	{
		QW = _mat[0][1] - _mat[1][0]; 
		QX = _mat[2][0] + _mat[0][2]; 
		QY = _mat[1][2] + _mat[2][1]; 
		QZ = tq[3];
	}

	s = sqrtf(0.25f / tq[j]);
	QW *= s;
	QX *= s;
	QY *= s;
	QZ *= s;

	return q;
}

//
// Sets translation component of the matrix
//
void Matrix::setTrans(VALUE_TYPE tx, VALUE_TYPE ty, VALUE_TYPE tz)
{
	_mat[3][0] = tx;
	_mat[3][1] = ty;
	_mat[3][2] = tz;
}

//
// Sets translation component of the matrix
//
void Matrix::setTrans(const Vector3 & v)
{
	_mat[3][0] = v[0];
	_mat[3][1] = v[1];
	_mat[3][2] = v[2];
}

//
// Sets this matrix to the result of matrix multiplication
//
void Matrix::mult(const Matrix & m1, const Matrix & m2)
{
	if (&m1 == this)
	{
		postMult(m2);
		return;
	}

	if (&m2 == this)
	{
		preMult(m1);
		return;
	}

	// PRECONDITION: We assume neither &m1 nor &m2 == this
	// if it did, use preMult or postMult instead

	_mat[0][0] = INNER_PRODUCT(m1, m2, 0, 0);
	_mat[0][1] = INNER_PRODUCT(m1, m2, 0, 1);
	_mat[0][2] = INNER_PRODUCT(m1, m2, 0, 2);
	_mat[0][3] = INNER_PRODUCT(m1, m2, 0, 3);
	_mat[1][0] = INNER_PRODUCT(m1, m2, 1, 0);
	_mat[1][1] = INNER_PRODUCT(m1, m2, 1, 1);
	_mat[1][2] = INNER_PRODUCT(m1, m2, 1, 2);
	_mat[1][3] = INNER_PRODUCT(m1, m2, 1, 3);
	_mat[2][0] = INNER_PRODUCT(m1, m2, 2, 0);
	_mat[2][1] = INNER_PRODUCT(m1, m2, 2, 1);
	_mat[2][2] = INNER_PRODUCT(m1, m2, 2, 2);
	_mat[2][3] = INNER_PRODUCT(m1, m2, 2, 3);
	_mat[3][0] = INNER_PRODUCT(m1, m2, 3, 0);
	_mat[3][1] = INNER_PRODUCT(m1, m2, 3, 1);
	_mat[3][2] = INNER_PRODUCT(m1, m2, 3, 2);
	_mat[3][3] = INNER_PRODUCT(m1, m2, 3, 3);
}

//
// Premultiplies this matrix by the specified matrix
//
void Matrix::preMult(const Matrix & other)
{
	VALUE_TYPE t[4];
	for (int col = 0; col < 4; ++col)
	{
		t[0] = INNER_PRODUCT(other, *this, 0, col);
		t[1] = INNER_PRODUCT(other, *this, 1, col);
		t[2] = INNER_PRODUCT(other, *this, 2, col);
		t[3] = INNER_PRODUCT(other, *this, 3, col);

		_mat[0][col] = t[0];
		_mat[1][col] = t[1];
		_mat[2][col] = t[2];
		_mat[3][col] = t[3];
	}
}

//
// Postmultiplies this matrix by the specified matrix
//
void Matrix::postMult(const Matrix & other)
{
	VALUE_TYPE t[4];

	for (int row = 0; row < 4; ++row)
	{
		t[0] = INNER_PRODUCT(*this, other, row, 0);
		t[1] = INNER_PRODUCT(*this, other, row, 1);
		t[2] = INNER_PRODUCT(*this, other, row, 2);
		t[3] = INNER_PRODUCT(*this, other, row, 3);
		SET_ROW(row, t[0], t[1], t[2], t[3])
	}
}


/*
osg::Matrixf::Matrixf( const osg::Matrixd& mat )
{
    set(mat.ptr());
}

osg::Matrixf& osg::Matrixf::operator = (const osg::Matrixd& rhs)
{
    set(rhs.ptr());
    return *this;
}

void osg::Matrixf::set(const osg::Matrixd& rhs)
{
    set(rhs.ptr());
}

// now compile up Matrix via Matrix_implementation
#include "Matrix_implementation.cpp"
*/
