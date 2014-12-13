#ifndef __MATH__RAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__RAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/trolltech.h>
#include <math/vector3.h>


/* Classes */

/** Ray. */
class Ray
{
public:
	/** Constructor. */
	inline Ray()
		: m_Origin(0.0f, 0.0f, 0.0f), m_Direction(0.0f, 1.0f, 0.0f)
	{
	}

	/**
	 * Constructor.
	 * @param origin Origin of the ray.
	 * @param dir Direction of the ray.
	 */
	inline Ray(const Vector3 & origin, const Vector3 & dir)
		: m_Origin(origin), m_Direction(dir)
	{
	}

	/**
	 * Sets origin of the ray.
	 * @param origin New origin of the ray.
	 */
	inline void setOrigin(const Vector3 & origin)
	{
		m_Origin = origin;
	}

	/**
	 * Sets direction of the ray.
	 * @param dir New direction of the ray.
	 */
	inline void setDirection(const Vector3 & dir)
	{
		m_Direction = dir;
	}

	/** Returns origin of the ray. */
	inline const Vector3 & origin() const
	{
		return m_Origin;
	}

	/** Returns direction of the ray. */
	inline const Vector3 & direction() const
	{
		return m_Direction;
	}

private:
	Vector3 m_Origin;		/**< Origin of the ray. */
	Vector3 m_Direction;	/**< Direction of the ray. */
};

Q_DECLARE_METATYPE(Ray);

#endif // __MATH__RAY_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
