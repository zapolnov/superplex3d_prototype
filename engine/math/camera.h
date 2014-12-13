#ifndef __MATH__CAMERA_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MATH__CAMERA_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>
#include <core/trolltech.h>
#include <math/vector3.h>
#include <math/plane.h>
#include <math/sphere.h>
#include <math/matrix.h>
#include <math/ray.h>


/* Constants */

/** Visibility of the bounding volume. */
enum BoundingVolumeVisibility
{
	bvNotVisible = 0,			/**< Bounding volume is not visible. */
	bvPartiallyVisible,			/**< Bounding volume is partially visible. */
	bvVisible					/**< Bounding volume is fully visible. */
};


/* Classes */

/** Perspective projection camera (frustum) class. */
class GCC_PUBLIC Camera : public QObject, public RefCounted
{
	Q_OBJECT
	Q_PROPERTY(Vector3 position READ position WRITE setPosition);
	Q_PROPERTY(Vector3 target READ target WRITE setTarget);
	Q_PROPERTY(Vector3 upVector READ upVector WRITE setUpVector);
	Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView);
	Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane);
	Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane);

public:
	/** Constructor. */
	MATHAPI Camera();

	/**
	 * Sets the near plane.
	 * @param zNear Near Z plane.
	 */
	MATHAPI void setNearPlane(float zNear);

	/**
	 * Sets the far plane.
	 * @param zFar Far Z plane.
	 */
	MATHAPI void setFarPlane(float zFar);

	/**
	 * Sets near and far planes.
	 * @param zNear Near Z plane.
	 * @param zFar Far Z plane.
	 */
	MATHAPI void setNearFarPlanes(float zNear, float zFar);

	/** Returns distance to the near plane of the frustum. */
	inline float nearPlane() const
	{
		return m_Near;
	}

	/** Returns distance to the far plane of the frustum. */
	inline float farPlane() const
	{
		return m_Far;
	}

	/**
	 * Sets vertical field of view.
	 * @param fov Vertical field of view (in degrees).
	 */
	MATHAPI void setFieldOfView(float fov);

	/** Returns vertical field of view (in degrees). */
	MATHAPI float fieldOfView() const;

	/** Returns the vertical field of view in radians divided by two. */
	inline float halfAngle() const
	{
		return m_HalfAngle;
	}

	/** Returns tangent of the vertical field of view in radians divided by two. */
	inline float halfAngleTangent() const
	{
		return m_HalfAngleTangent;
	}

	/**
	 * Sets camera position.
	 * @param pos New position of the camera.
	 */
	MATHAPI void setPosition(const Vector3 & pos);

	/**
	 * Sets coordinates of the camera target.
	 * @param target New target of the camera.
	 */
	MATHAPI void setTarget(const Vector3 & target);

	/**
	 * Sets up vector of the camera.
	 * @param up New up vector of the camera.
	 */
	MATHAPI void setUpVector(const Vector3 & up);

	/**
	 * Sets camera transformation.
	 * @param pos Camera position.
	 * @param target Camera target.
	 * @param up Up vector.
	 */
	MATHAPI void lookAt(const Vector3 & pos, const Vector3 & target, const Vector3 & up);

	/** Returns camera position. */
	inline const Vector3 & position() const
	{
		return m_Position;
	}

	/** Returns camera target. */
	inline Vector3 target() const
	{
		return m_Position + m_Forward;
	}

	/** Returns camera up vector. */
	inline const Vector3 & upVector() const
	{
		return m_UpVector;
	}

	/** Returns camera forward vector (not normalized). */
	inline const Vector3 & forwardVector() const
	{
		return m_Forward;
	}

	/** Returns camera side vector. */
	inline const Vector3 & sideVector() const
	{
		return m_Side;
	}

	/**
	 * Returns visibility of the specified sphere.
	 * @param viewportSize Size of the viewport.
	 * @param sphere Sphere to check.
	 * @param distance Pointer to the variable where distance to camera will be stored.
	 */
	MATHAPI BoundingVolumeVisibility sphereInFrustum(const QSize & viewportSize, const Sphere & sphere,
		float * pDistance = NULL) const;

	/**
	 * Unprojects 2D point from viewport into the ray in 3D space.
	 * @param viewportSize Size of the viewport.
	 * @param point Point on screen.
	 */
	MATHAPI Ray unprojectPoint(const QSize & viewportSize, const QPoint & point) const;

	/** Returns matrix that describes the camera's transformation. */
	inline const Matrix & transformationMatrix() const
	{
		if (unlikely(m_TransformDirty))
			const_cast<Camera *>(this)->calcTransformation();

		return m_Transform;
	}

	/** Returns inverse transformation matrix for this camera. */
	inline const Matrix & invTransformationMatrix() const
	{
		if (unlikely(m_InvTransformDirty))
			const_cast<Camera *>(this)->calcInverseTransformation();

		return m_InvTransform;
	}

	/**
	 * Returns the projection matrix.
	 * @param viewportSize Size of the viewport.
	 */
	inline const Matrix & projectionMatrix(const QSize & viewportSize) const
	{
		if (unlikely(viewportSize != m_ViewportSize))
		{
			const_cast<Camera *>(this)->m_ProjectionDirty = true;
			const_cast<Camera *>(this)->m_ModelViewProjDirty = true;
			const_cast<Camera *>(this)->m_PlanesDirty = true;
			const_cast<Camera *>(this)->m_ViewportSize = viewportSize;
		}

		if (unlikely(m_ProjectionDirty))
			const_cast<Camera *>(this)->calcProjection();

		return m_Projection;
	}

	/**
	 * Returns the combined transformation & projection matrix.
	 * @param viewportSize Size of the viewport.
	 */
	inline const Matrix & combinedMatrix(const QSize & viewportSize) const
	{
		if (unlikely(viewportSize != m_ViewportSize))
		{
			const_cast<Camera *>(this)->m_ProjectionDirty = true;
			const_cast<Camera *>(this)->m_ModelViewProjDirty = true;
			const_cast<Camera *>(this)->m_PlanesDirty = true;
			const_cast<Camera *>(this)->m_ViewportSize = viewportSize;
		}

		if (unlikely(m_TransformDirty || m_ProjectionDirty))
		{
			if (unlikely(m_TransformDirty))
				const_cast<Camera *>(this)->calcTransformation();
			if (unlikely(m_ProjectionDirty))
				const_cast<Camera *>(this)->calcProjection();
		}

		if (unlikely(m_ModelViewProjDirty))
		{
			const_cast<Camera *>(this)->m_ModelViewProj = m_Transform * m_Projection;
			const_cast<Camera *>(this)->m_ModelViewProjDirty = false;
		}

		return m_ModelViewProj;
	}

private:
	/** Plane index. */
	enum PlaneIndex
	{
		NearPlane = 0,				/**< Near plane of the frustum. */
		FarPlane,					/**< Far plane of the frustum. */
		TopPlane,					/**< Top plane of the frustum. */
		BottomPlane,				/**< Bottom plane of the frustum. */
		LeftPlane,					/**< Left plane of the frustum. */
		RightPlane,					/**< Right plane of the frustum. */
	};

	// Field of view
	float m_HalfAngle;				/**< Vertical field of view (in radians) divided by two. */
	float m_HalfAngleTangent;		/**< Tangent of the m_HalfAngle. */

	// Near and far planes
	float m_Near;					/**< Near plane. */
	float m_Far;					/**< Far plane. */

	// Viewport
	QSize m_ViewportSize;			/**< Size of the viewport. */

	// Camera transformations
	Vector3 m_Position;				/**< Camera position. */
	Vector3 m_Forward;				/**< Camera forward vector. */
	Vector3 m_Side;					/**< Camera side vector. */
	Vector3 m_UpVector;				/**< Camera up vector. */

	bool m_ProjectionDirty : 1;		/**< True, if projection matrix should be recalculated. */
	bool m_TransformDirty : 1;		/**< True, if transformation matrix should be recalculated. */
	bool m_PlanesDirty : 1;			/**< True, if planes should be recalculated. */
	bool m_InvTransformDirty : 1;	/**< True, if inverse transformation matrix should be recalculated. */
	bool m_ModelViewProjDirty : 1;	/**< True, if combined matrix needs to be recalculated. */

	Matrix m_Transform;				/**< Transformation matrix. */
	Matrix m_InvTransform;			/**< Inverse transformation matrix. */
	Matrix m_Projection;			/**< Projection matrix. */
	Matrix m_ModelViewProj;			/**< Combined transformation/projection matrix. */
	Plane m_Planes[6];				/**< Frustum planes. */

	/** Calculates projection matrix. */
	MATHAPI void calcProjection();

	/** Calculates frustum planes. */
	MATHAPI void calcPlanes();

	/** Calculates transformation matrix. */
	MATHAPI void calcTransformation();

	/** Calculates inverse transformation matrix. */
	MATHAPI void calcInverseTransformation();
};

/** Pointer to the camera. */
typedef Pointer<Camera> CameraPtr;

#endif // __MATH__CAMERA_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
