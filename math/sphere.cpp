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
#include <math/sphere.h>
#include <math/ray.h>

/* Sphere methods */

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRay(const Ray & ray) const
{
	// Ray-sphere intersection test from Graphics Gems p.388
	// **NOTE** There is a bug in this Graphics Gem.  If the origin
	// of the ray is *inside* the sphere being tested, it reports the
	// wrong intersection location.  This code has a fix for the bug.

	// Notation:
	// point E = rayOrigin
	// point O = sphere center

	Vector3 EO = m_Center - ray.origin();
	Vector3 V = ray.direction();

	float dist2 = EO.x() * EO.x() + EO.y() * EO.y() + EO.z() * EO.z();

	// Bug Fix For Gem, if origin is *inside* the sphere, invert the
	// direction vector so that we get a valid intersection location.
	if (dist2 < m_Radius2)
		V *= -1.0f;

	float v = EO * V;
	float disc = m_Radius2 - (EO.length2() - v * v);

	return disc > 0.0f;
}

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRay(const Ray & ray, Vector3 & intersect) const
{
	// Ray-sphere intersection test from Graphics Gems p.388
	// **NOTE** There is a bug in this Graphics Gem.  If the origin
	// of the ray is *inside* the sphere being tested, it reports the
	// wrong intersection location.  This code has a fix for the bug.

	// Notation:
	// point E = rayOrigin
	// point O = sphere center

	Vector3 EO = m_Center - ray.origin();
	Vector3 V = ray.direction();

	float dist2 = EO.x() * EO.x() + EO.y() * EO.y() + EO.z() * EO.z();

	// Bug Fix For Gem, if origin is *inside* the sphere, invert the
	// direction vector so that we get a valid intersection location.
	if (dist2 < m_Radius2)
		V *= -1.0f;

	float v = EO * V;
	float disc = m_Radius2 - (EO.length2() - v * v);

	if (disc > 0.0f)
	{
		float d = sqrtf(disc);
//		float dist2 = (m_Center - ray.origin()).length2();      // FIXME?
		intersect = ray.origin() + V * (v - d);

		return true;
	}

	return false;
}

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRayInFront(const Ray & ray, float distance) const
{
	Vector3 sect;

	bool hit = intersectsRayInFront(ray, sect);
	if (hit)
	{
		float d = (sect - ray.origin()).length2();
		return d <= distance * distance;
	}

	return false;
}

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRayInFront(const Ray & ray, float distance, Vector3 & intersect) const
{
	Vector3 sect;

	bool hit = intersectsRayInFront(ray, sect);
	if (hit)
	{
		float d = (sect - ray.origin()).length2();

		if (d > distance * distance)
			return false;
		intersect = sect;

		return true;
	}

	return false;
}

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRayInFront(const Ray & ray, Vector3 & intersect) const
{
	Vector3 sect;

	bool hit = intersectsRay(ray, sect);
	if (hit)
	{
		Vector3 V = sect - ray.origin();
		float dot = V * ray.direction();

		if (dot >= 0)
		{
			// It's in front!
			intersect = sect;
			return true;
		}
	}

	return false;
}

//
// Returns true if sphere intersects the specified ray
//
bool Sphere::intersectsRayInFront(const Ray & ray) const
{
	Vector3 sect;

	bool hit = intersectsRay(ray, sect);
	if (hit)
	{
		Vector3 V = sect - ray.origin();
		float dot = V * ray.direction();

		if (dot >= 0)
			return true;
	}

	return false;
}

//
// Combines this sphere with the specified one and returns the combined bounding sphere
//
Sphere Sphere::combineWith(const Sphere & sphere2)
{
	Vector3 v = sphere2.m_Center - m_Center;
	float vlen = v.length();

	return Sphere
	(
		m_Center + (0.5f * (sphere2.m_Radius + vlen - m_Radius) / vlen) * v,
		0.5f * (m_Radius + vlen + sphere2.m_Radius)
	);
}
