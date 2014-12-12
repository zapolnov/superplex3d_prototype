#include <math/plane.h>

/* Plane methods */

//
// Constructor
//
Plane::Plane(const Vector3 & v1, const Vector3 & v2, const Vector3 & v3)
	: m_Normal((v1 - v2) ^ (v3 - v2)), m_Point(v2)
{
	m_Normal.normalize();
	m_D = -(m_Normal * m_Point);
}

//
// Constructor
//
Plane::Plane(const Vector3 & n, const Vector3 & p)
	: m_Normal(n), m_Point(p)
{
	m_Normal.normalize();
	m_D = -(m_Normal * m_Point);
}

//
// Normalizes the plane equation
//
void Plane::normalize()
{
	float mag = m_Normal.normalize();
	m_D /= mag;
}
