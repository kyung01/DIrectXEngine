#include <Graphic\Scene\Camera.h>
using namespace  NGraphic::NScene;



Camera::Camera() :
	m_clipFar(500),
	m_clipNear(0.1),
	m_screenWidth(100),
	m_screenHeight(100),
	m_fov(3.14*0.5)
{
	m_pos = Vector3(0, 0, 0);
	m_isDirty_matView = true;
	m_isDirty_matProjection = true;
}

float Camera::getFOV()
{
	return m_fov;
}

Matrix Camera::getProjectionMatrix()
{
	if (m_isDirty_matProjection) {
		m_isDirty_matProjection = false;
		m_matProjection = DirectX::XMMatrixPerspectiveFovLH(
			m_fov,		// Field of View Angle
			m_screenWidth / m_screenHeight,		// Aspect ratio
			m_clipNear, m_clipFar);					// Far clip plane distance
	}
	return m_matProjection;
}
Matrix Camera::getProjectionMatrix(float width, float height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_isDirty_matProjection = true;
	return getProjectionMatrix();
}

Matrix Camera::getProjectionMatrix(float fov, float screen_width, float screen_height, float clipNear, float clipFar)
{
	if (m_fov != fov || m_screenWidth != screen_width || m_screenHeight != screen_height || m_clipNear != clipNear || m_clipFar != clipFar) {
		m_isDirty_matProjection = true;
		m_screenWidth = screen_width;
		m_screenHeight = screen_height;
		m_clipNear = clipNear;
		m_clipFar = clipFar;
		m_fov = fov;
	}
	//DirectX::SimpleMath::Matrix::CreatePerspective()
	return getProjectionMatrix();
}



Matrix Camera::getViewMatrix()
{
	if (m_isDirty_matView) {
		m_isDirty_matView = false;
		//std::cout << "DEBUG " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
		//std::cout << "rotatedFOrward " << rotatedFOrward.x << " " << rotatedFOrward.y << " " << rotatedFOrward.z << "\n";
		//Vector3 target =  (Vector3)DirectX::XMVector3Rotate(Vector3(0,0,1), m_rotation);// , 0.0f);
		m_matView = DirectX::XMMatrixLookToLH(m_pos, m_dirLook, Vector3::Up);
	}
	return m_matView;
}


void Camera::setFOV(float value)
{
	m_isDirty_matProjection = true;
	m_fov = value;
}

IObject Camera::setPos(Vector3 pos)
{
	IObject::setPos(pos);
	m_isDirty_matView = true;
	return *this;
}


IObject Camera::setRotation(Quaternion quaternion)
{
	IObject::setRotation(quaternion);
	m_isDirty_matView = true;
	return *this;

}