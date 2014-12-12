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
#include <math/quaternion.h>
#include <math/matrix.h>

/* Quaternion methods */

//
// Sets quaternion from the specified matrix
//
void Quaternion::setFromMatrix(const Matrix & m)
{
	*this = m.getRotate();
}

//
// Updates matrix rotation from this quaternion
//
void Quaternion::getMatrix(Matrix & m) const
{
	m.makeRotate(*this);
}

//
// Sets quaternion rotation from axis and angle
//
void Quaternion::makeRotate(VALUE_TYPE angle, VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z)
{
	const VALUE_TYPE epsilon = 0.0000001f;

	VALUE_TYPE length = sqrt(x * x + y * y + z * z);
	if (length < epsilon)
	{
		*this = Quaternion();
		return;
	}

	VALUE_TYPE inversenorm = 1.0f / length;
	VALUE_TYPE coshalfangle = cos(0.5f * angle);
	VALUE_TYPE sinhalfangle = sin(0.5f * angle);

	_v[0] = x * sinhalfangle * inversenorm;
	_v[1] = y * sinhalfangle * inversenorm;
	_v[2] = z * sinhalfangle * inversenorm;
	_v[3] = coshalfangle;
}

//
// Sets quaternion rotation from axis and angle
//
void Quaternion::makeRotate(VALUE_TYPE angle, const Vector3 & vec)
{
	makeRotate(angle, vec._v[0], vec._v[1], vec._v[2]);
}

//
// Sets quaternion rotation from three rotations
//
void Quaternion::makeRotate(VALUE_TYPE angle1, const Vector3 & axis1, 
	VALUE_TYPE angle2, const Vector3 & axis2, VALUE_TYPE angle3, const Vector3 & axis3)
{
	Quaternion q1, q2, q3;
	q1.makeRotate(angle1, axis1);
	q2.makeRotate(angle2, axis2);
	q3.makeRotate(angle3, axis3);

	*this = q1 * q2 * q3;
}

//
// Makes a quaternion which will rotate from first vector to the second
//
void Quaternion::makeRotate(const Vector3 & from, const Vector3 & to)
{
	Vector3 sourceVector = from;
	Vector3 targetVector = to;

	VALUE_TYPE fromLen2 = from.length2();
	VALUE_TYPE fromLen;
	if (fromLen2 >= 1.0 - 1e-7 && fromLen2 <= 1.0 + 1e-7)
		fromLen = 1.0f;
	else
	{
		fromLen = sqrtf(fromLen2);
		sourceVector /= fromLen;
	}

	VALUE_TYPE toLen2 = to.length2();
	if (toLen2 < 1.0 - 1e-7 || toLen2 > 1.0 + 1e-7)
	{
		VALUE_TYPE toLen;
		if (toLen2 > fromLen2 - 1e-7 && toLen2 < fromLen2 + 1e-7)
			toLen = fromLen;
		else
			toLen = sqrtf(toLen2);
		targetVector /= toLen;
	}

    // Use "dot product plus one" as test as it can be re-used later on
	float dotProdPlus1 = 1.0f + sourceVector * targetVector;

	// Check for degenerate case of full u-turn. Use epsilon for detection
	if (dotProdPlus1 < 1e-7)
	{
		// Get an orthogonal vector of the given vector
		// in a plane with maximum vector coordinates.
		// Then use it as quaternion axis with pi angle
		// Trick is to realize one value at least is >0.6 for a normalized vector.
		if (fabs(sourceVector._v[0]) < 0.6f)
		{
			const float norm = sqrtf(1.0f - sourceVector._v[0] * sourceVector._v[0]);
			_v[0] = 0.0f;
			_v[1] = sourceVector._v[2] / norm;
			_v[2] = -sourceVector._v[1] / norm;
			_v[3] = 0.0f;
		}
		else if (fabs(sourceVector._v[1]) < 0.6f)
		{
			const float norm = sqrtf(1.0f - sourceVector._v[1] * sourceVector._v[1]);
			_v[0] = -sourceVector._v[2] / norm;
			_v[1] = 0.0f;
			_v[2] = sourceVector._v[0] / norm;
			_v[3] = 0.0f;
		}
		else
		{
			const float norm = sqrtf(1.0f - sourceVector._v[2] * sourceVector._v[2]);
			_v[0] = sourceVector._v[1] / norm;
			_v[1] = -sourceVector._v[0] / norm;
			_v[2] = 0.0f;
			_v[3] = 0.0f;
		}
	}
	else
	{
		// Find the shortest angle quaternion that transforms normalized vectors
		// into one other. Formula is still valid when vectors are colinear
		const float s = sqrtf(0.5f * dotProdPlus1);
		const Vector3 tmp = sourceVector ^ targetVector / (2.0f * s);
		_v[0] = tmp._v[0];
		_v[1] = tmp._v[1];
		_v[2] = tmp._v[2];
		_v[3] = s;
	}
}

//
// Return the angle and vector components represented by the quaternion
//
void Quaternion::getRotate(VALUE_TYPE & angle, VALUE_TYPE & x, VALUE_TYPE & y, VALUE_TYPE & z) const
{
	VALUE_TYPE sinhalfangle = sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);

	angle = 2.0f * atan2f(sinhalfangle, _v[3]);
	if (sinhalfangle != 0.0f)
	{
		x = _v[0] / sinhalfangle;
		y = _v[1] / sinhalfangle;
		z = _v[2] / sinhalfangle;
	}
	else
	{
		x = 0.0f;
		y = 0.0f;
		z = 1.0f;
	}
}

//
// Return the angle and vector components represented by the quaternion
//
void Quaternion::getRotate(VALUE_TYPE & angle, Vector3 & vec) const
{
	getRotate(angle, vec._v[0], vec._v[1], vec._v[2]);
}

//
// Spherical Linear Interpolation
//
void Quaternion::slerp(VALUE_TYPE t, const Quaternion & from, const Quaternion & to)
{
    float omega, cosomega, sinomega, scale_from, scale_to;
	const float epsilon = 0.00001f;

	Quaternion quatTo(to);

	cosomega = from.asVec4() * to.asVec4();
	if (cosomega < 0.0f)
	{
		cosomega = -cosomega; 
		quatTo = -to;
	}

	if (1.0f - cosomega > epsilon)
    {
		omega = acosf(cosomega);	// 0 <= omega <= Pi (see man acos)
		sinomega = sinf(omega);		// this sinomega should always be +ve so
		scale_from = sin((1.0f - t) * omega) / sinomega;
		scale_to = sin(t * omega) / sinomega;
	}
	else
	{
		scale_from = 1.0f - t;
		scale_to = t;
	}

	*this = (from * scale_from) + (quatTo * scale_to);
}
