#include <Engine\Camera.h>
using namespace KEngine;
Camera::Camera(){
	//dirLook = Vector3(0, 0, 1);
	m_isDirtyView = true;
	//m_isDirtyProj = true;
	setProjParameters(3.14f / 2.0f, 100, 100, 0.1, 1000);
}
Matrix Camera::getViewMatrix()
{
	if (m_isDirtyView) {
		Vector3 dirLook = getLookDir();
		viewMatrix = DirectX::XMMatrixLookToLH(position, dirLook, Vector3::Up);
		m_isDirtyView = false;
	}
	return viewMatrix;
}Matrix Camera::getProjMatrix()
{
	if (m_isDirtyProj) {
		m_isDirtyProj = false;
		projMatrix = DirectX::XMMatrixPerspectiveFovLH(
			m_fov,		// Field of View Angle
			m_screenWidth / m_screenHeight,		// Aspect ratio
			m_clipNear, m_clipFar);
	}
	return Matrix();
}
void Camera::setProjParameters(float fov, float screenWidth, float screenHeight, float clipNear, float clipFar)
{
	m_fov = fov;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_clipNear = clipNear;
	m_clipFar = clipFar;
	m_isDirtyProj = true;
}
Vector3 KEngine::Camera::getPosition()
{
	return position;
}
Vector3 KEngine::Camera::getLookDir()
{
	return DirectX::XMVector3Rotate(Vector3(0, 0, 1), rotation);
}
void KEngine::Camera::setPosition(Vector3 position)
{
	this->position = position;
	m_isDirtyView = true;
}
void KEngine::Camera::setQuaternion(Quaternion rotation)
{
	this->rotation = rotation;
	m_isDirtyView = true;
}