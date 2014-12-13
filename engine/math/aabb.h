#ifndef __MATH__AABB_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__AABB_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <math/vector3.h>


/* Classes */

/** Axis-aligned bounding box. */
class AABB
{
public:
	/** Constructor. */
	inline AABB()
		: m_Min(0.0f, 0.0f, 0.0f), m_Max(0.0f, 0.0f, 0.0f)
	{
	}

	/**
	 * Constructor.
	 * @param min Minimum point.
	 * @param max Maximum point.
	 */
	inline AABB(const Vector3 & min, const Vector3 & max)
		: m_Min(min), m_Max(max)
	{
	}

	/** Returns minimum point. */
	inline const Vector3 & minPoint() const
	{
		return m_Min;
	}

	/** Returns maximum point. */
	inline const Vector3 & maxPoint() const
	{
		return m_Max;
	}

private:
	Vector3	m_Min;		/**< Minimum point. */
	Vector3	m_Max;		/**< Maximum point. */
};

#endif // __MATH__AABB_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
