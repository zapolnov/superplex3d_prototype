/*
 * Copyright (C) 2009 PolarGears, Inc.
 * Portions Copyright (C) John W. Ratcliff, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) John W. Ratcliff, 2001"
 */
#ifndef __MATH__SPHERE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__SPHERE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>


/* Classes */

class Ray;

/** Sphere in 3 dimensions. */
class Sphere
{
public:
	/** Constructor. */
	inline Sphere()
		: m_Center(0.0f, 0.0f, 0.0f), m_Radius(0.0f), m_Radius2(0.0f)
	{
	}

	/**
	 * Constructor.
	 * @param center Coordinates of the sphere center.
	 * @param radius Radius of the sphere.
	 */
	inline Sphere(const Vector3 & center, float radius)
		: m_Center(center), m_Radius(radius), m_Radius2(radius * radius)
	{
	}

	/**
	 * Sets coordinates of the center of the sphere and it's radius.
	 * @param center Coordinates of the sphere's center.
     * @param radius Radius to set.
	 */
	inline void set(const Vector3 & center, float radius)
	{
		m_Center = center;
		m_Radius = radius;
		m_Radius2 = radius * radius;
	}

	/**
	 * Sets coordinates of the center of the sphere.
	 * @param center Coordinates of the sphere's center.
	 */
	inline void setCenter(const Vector3 & center)
	{
		m_Center = center;
	}

	/**
	 * Sets radius of the sphere.
	 * @param radius Radius to set.
	 */
	inline void setRadius(float radius)
	{
		m_Radius = radius;
		m_Radius2 = radius * radius;
	}

	/** Returns radius of the sphere. */
	inline float radius() const
	{
		return m_Radius;
	}

	/** Returns squared radius of the sphere. */
	inline float radius2() const
	{
		return m_Radius2;
	}

	/** Returns coordinates of the sphere center. */
	const Vector3 & center() const
	{
		return m_Center;
	}

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 */
	MATHAPI bool intersectsRay(const Ray & ray) const;

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 * @param intersect Reference to the variable, where coordinates of intersection point will be stored.
	 */
	MATHAPI bool intersectsRay(const Ray & ray, Vector3 & intersect) const;

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 * @param distance Length of the ray.
	 */
	MATHAPI bool intersectsRayInFront(const Ray & ray, float distance) const;

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 * @param distance Length of the ray.
	 * @param intersect Reference to the variable, where coordinates of intersection point will be stored.
	 */
	MATHAPI bool intersectsRayInFront(const Ray & ray, float distance, Vector3 & intersect) const;

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 * @param intersect Reference to the variable, where coordinates of intersection point will be stored.
	 */
	MATHAPI bool intersectsRayInFront(const Ray & ray, Vector3 & intersect) const;

	/**
	 * Returns true if sphere intersects the specified ray.
	 * @param ray Ray.
	 */
	MATHAPI bool intersectsRayInFront(const Ray & ray) const;

	/**
	 * Combines this sphere with the specified one and returns the combined bounding sphere.
	 * @param sphere2 Second sphere.
	 */
	MATHAPI Sphere combineWith(const Sphere & sphere2);

private:
	Vector3 m_Center;	/**< Coordinates of the sphere center. */
	float m_Radius;		/**< Radius of the sphere. */
	float m_Radius2;	/**< Squared radius of the sphere. */
};

Q_DECLARE_METATYPE(Sphere);

#endif // __MATH__SPHERE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
