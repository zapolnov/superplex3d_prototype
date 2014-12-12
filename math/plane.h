#ifndef __MATH__PLANE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__PLANE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <math/vector3.h>


/* Classes */

/** Plane. */
class Plane
{
public:
	/** Constructor. */
	inline Plane()
	{
	}

	/** 
	 * Constructor.
	 * @param v1 First point.
	 * @param v2 Second point.
	 * @param v3 Third point.
	 */
	MATHAPI Plane(const Vector3 & v1, const Vector3 & v2, const Vector3 & v3);

	/** 
	 * Constructor.
	 * @param n Normal vector.
	 * @param p Point.
	 */
	MATHAPI Plane(const Vector3 & n, const Vector3 & p);

	/** 
	 * Constructor.
	 * @param a Coefficient A of the plane equation.
	 * @param b Coefficient B of the plane equation.
	 * @param c Coefficient C of the plane equation.
	 * @param d Coefficient D of the plane equation.
	 */
	inline Plane(float a, float b, float c, float d)
		: m_Normal(a, b, c), m_D(d)
	{
	}

	/** Normalizes the plane equation. */
	MATHAPI void normalize();

	/**
	 * Returns distance between the plane and the specified point.
	 * @param p Point.
	 */
	inline float distance(const Vector3 & p) const
	{
		return m_D + m_Normal * p;
	}

	/** Returns coefficient A of the plane equation. */
	inline float A() const
	{
		return m_Normal.x();
	}

	/** Returns coefficient B of the plane equation. */
	inline float B() const
	{
		return m_Normal.y();
	}

	/** Returns coefficient C of the plane equation. */
	inline float C() const
	{
		return m_Normal.z();
	}

	/** Returns coefficient D of the plane equation. */
	inline float D() const
	{
		return m_D;
	}

private:
	Vector3 m_Normal;	/**< Plane normal (coefficients A, B and C). */
	float m_D;			/**< Plane D coefficient. */
	Vector3 m_Point;	/**< Point. */
};

#endif // __MATH__PLANE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
