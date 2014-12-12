#include <math/camera.h>

/* Camera methods */

//
// Constructor
//
Camera::Camera()
	: m_ProjectionDirty(true), m_TransformDirty(true), m_PlanesDirty(true), m_InvTransformDirty(true),
	  m_ModelViewProjDirty(true)
{
	setNearFarPlanes(1.0f, 10.0f);
	setFieldOfView(45.0f);
	lookAt(Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

//
// Sets the near plane
//
void Camera::setNearPlane(float zNear)
{
	m_Near = zNear;

	m_ProjectionDirty = true;
	m_ModelViewProjDirty = true;
	m_PlanesDirty = true;
}

//
// Sets the far plane
//
void Camera::setFarPlane(float zFar)
{
	m_Far = zFar;

	m_ProjectionDirty = true;
	m_ModelViewProjDirty = true;
	m_PlanesDirty = true;
}

//
// Sets near and far planes
//
void Camera::setNearFarPlanes(float zNear, float zFar)
{
	m_Near = zNear;
	m_Far = zFar;

	m_ProjectionDirty = true;
	m_ModelViewProjDirty = true;
	m_PlanesDirty = true;
}

//
// Sets vertical field of view
//
void Camera::setFieldOfView(float fov)
{
	m_HalfAngle = (float)(fov * PI / 360.0);
	m_HalfAngleTangent = tanf(m_HalfAngle);

	m_ProjectionDirty = true;
	m_ModelViewProjDirty = true;
	m_PlanesDirty = true;
}

//
// Returns vertical field of view (in degrees)
//
float Camera::fieldOfView() const
{
	return (float)(m_HalfAngle * 360.0 / PI);
}

//
// Sets camera position
//
void Camera::setPosition(const Vector3 & pos)
{
	lookAt(pos, target(), upVector());
}

//
// Sets coordinates of the camera target
//
void Camera::setTarget(const Vector3 & target)
{
	lookAt(position(), target, upVector());
}

//
// Sets up vector of the camera
//
void Camera::setUpVector(const Vector3 & up)
{
	lookAt(position(), target(), up);
}

//
// Sets camera transformation
//
void Camera::lookAt(const Vector3 & pos, const Vector3 & target, const Vector3 & up)
{
	m_Position = pos;

	m_Forward = target - pos;
	Vector3 fwd = m_Forward;
	fwd.normalize();

	m_Side = fwd ^ up;
	m_Side.normalize();

	m_UpVector = m_Side ^ fwd;
	m_UpVector.normalize();

	m_PlanesDirty = true;
	m_InvTransformDirty = true;
	m_TransformDirty = true;
	m_ModelViewProjDirty = true;
}

//
// Returns true if the specified sphere is inside the camera's frustum
//
BoundingVolumeVisibility Camera::sphereInFrustum(const QSize & viewportSize,
	const Sphere & sphere, float * pDistance) const
{
	float distance;

	if (unlikely(viewportSize != m_ViewportSize))
	{
		const_cast<Camera *>(this)->m_ProjectionDirty = true;
		const_cast<Camera *>(this)->m_ModelViewProjDirty = true;
		const_cast<Camera *>(this)->m_PlanesDirty = true;
		const_cast<Camera *>(this)->m_ViewportSize = viewportSize;
	}

	if (unlikely(m_PlanesDirty))
		const_cast<Camera *>(this)->calcPlanes();

	const Vector3 & center = sphere.center();
	float radius = sphere.radius();

	BoundingVolumeVisibility vis = bvVisible;
	for (int i = 0; i < 6; i++)
	{
		distance = m_Planes[i].distance(center);

		if (i == 0 && pDistance)
		{
			Q_ASSERT(NearPlane == 0);
			*pDistance = distance;
		}

		if (distance < -radius)
			return bvNotVisible;
		else if (distance < radius)
			vis = bvPartiallyVisible;
	}

	return vis;
}

//
// Unprojects 2D point from viewport into the ray in 3D space
//
Ray Camera::unprojectPoint(const QSize & viewportSize, const QPoint & point) const
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
	if (unlikely(m_InvTransformDirty))
		const_cast<Camera *>(this)->calcInverseTransformation();

	Vector3 v
	(
		 (((2.0f * (float)point.x()) / (float)m_ViewportSize.width())  - 1.0f) / m_Projection(0, 0),
		-(((2.0f * (float)point.y()) / (float)m_ViewportSize.height()) - 1.0f) / m_Projection(1, 1),
		-1.0f
	);

	Vector3 dir
	(
		v.x() * m_InvTransform(0, 0) + v.y() * m_InvTransform(1, 0) + v.z() * m_InvTransform(2, 0),
		v.x() * m_InvTransform(0, 1) + v.y() * m_InvTransform(1, 1) + v.z() * m_InvTransform(2, 1),
		v.x() * m_InvTransform(0, 2) + v.y() * m_InvTransform(1, 2) + v.z() * m_InvTransform(2, 2)
	);
	dir.normalize();

	return Ray(m_InvTransform.getTrans(), dir);
}

//
// Calculates projection matrix
//
void Camera::calcProjection()
{
	float deltaZ = -1.0f / (m_Far - m_Near);
	float sine = sinf(m_HalfAngle);

	float aspectRatio;
	if (likely(m_ViewportSize.width() > 0 && m_ViewportSize.height() > 0))
		aspectRatio = (float)m_ViewportSize.width() / (float)m_ViewportSize.height();
	else
		aspectRatio = 1.0f;

	if (unlikely(deltaZ == 0.0f || sine == 0.0f || aspectRatio == 0.0f))
		m_Projection.makeIdentity();
	else
	{
		float cotangent = cosf(m_HalfAngle) / sine;

		m_Projection = Matrix
		(
			cotangent / aspectRatio, 0.0f, 0.0f, 0.0f,
			0.0f, cotangent, 0.0f, 0.0f,
			0.0f, 0.0f, (m_Far + m_Near) * deltaZ, -1.0f,
			0.0f, 0.0f, 2.0f * m_Near * m_Far * deltaZ, 0.0f
		);
	}

	m_ProjectionDirty = false;
}

//
// Calculates frustum planes
//
void Camera::calcPlanes()
{
	const Matrix & m = combinedMatrix(m_ViewportSize);

	m_Planes[NearPlane]		= Plane(m(0, 3) + m(0, 2), m(1, 3) + m(1, 2), m(2, 3) + m(2, 2), m(3, 3) + m(3, 2));
	m_Planes[FarPlane]		= Plane(m(0, 3) - m(0, 2), m(1, 3) - m(1, 2), m(2, 3) - m(2, 2), m(3, 3) - m(3, 2));
	m_Planes[TopPlane]		= Plane(m(0, 3) - m(0, 1), m(1, 3) - m(1, 1), m(2, 3) - m(2, 1), m(3, 3) - m(3, 1));
	m_Planes[BottomPlane]	= Plane(m(0, 3) + m(0, 1), m(1, 3) + m(1, 1), m(2, 3) + m(2, 1), m(3, 3) + m(3, 1));
	m_Planes[LeftPlane]		= Plane(m(0, 3) + m(0, 0), m(1, 3) + m(1, 0), m(2, 3) + m(2, 0), m(3, 3) + m(3, 0));
	m_Planes[RightPlane]	= Plane(m(0, 3) - m(0, 0), m(1, 3) - m(1, 0), m(2, 3) - m(2, 0), m(3, 3) - m(3, 0));

	m_Planes[NearPlane].normalize();
	m_Planes[FarPlane].normalize();
	m_Planes[TopPlane].normalize();
	m_Planes[BottomPlane].normalize();
	m_Planes[LeftPlane].normalize();
	m_Planes[RightPlane].normalize();

	m_PlanesDirty = false;
}

//
// Calculates transformation matrix
//
void Camera::calcTransformation()
{
	Vector3 fwd = -m_Forward;
	fwd.normalize();

	m_Transform = Matrix::translate(-m_Position) * Matrix
	(
		m_Side.x(), m_UpVector.x(), fwd.x(), 0.0f,
		m_Side.y(), m_UpVector.y(), fwd.y(), 0.0f,
		m_Side.z(), m_UpVector.z(), fwd.z(), 0.0f,
		0.0f,       0.0f,           0.0f,    1.0f
	);

	m_TransformDirty = false;
	m_InvTransformDirty = true;
	m_ModelViewProjDirty = true;
}

//
// Calculates inverse transformation matrix
//
void Camera::calcInverseTransformation()
{
	if (unlikely(m_TransformDirty))
		calcTransformation();

	m_InvTransform.invert(m_Transform);
	m_InvTransformDirty = false;
}
