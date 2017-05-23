#include <Graphic\Scene\SphericalCamera.h>
using namespace NGraphic::NScene;


void SphericalCamera::updatePointLightViewMatrixs()
{
	Camera::getViewMatrix();
	m_matLookXPlus = DirectX::XMMatrixLookToLH(m_pos, Vector3(1, 0, 0), Vector3(0, 1, 0));
	m_matLookXMinus = DirectX::XMMatrixLookToLH(m_pos, Vector3(-1, 0, 0), Vector3(0, 1, 0));
	m_matLookYPlus = DirectX::XMMatrixLookToLH(m_pos, Vector3(0, 1, 0), Vector3(0, 0, -1));
	m_matLookYMinus = DirectX::XMMatrixLookToLH(m_pos, Vector3(0, -1, 0), Vector3(0, 0, 1));
	m_matLookZPlus = DirectX::XMMatrixLookToLH(m_pos, Vector3(0, 0, 1), Vector3(0, 1, 0));
	m_matLookZMinus = DirectX::XMMatrixLookToLH(m_pos, Vector3(0, 0, -1), Vector3(0, 1, 0));
}

SphericalCamera::SphericalCamera()
{
	Camera();
	setFOV(3.14 / 2 + 0.15f);
}


Matrix SphericalCamera::getViewMatrix()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		//update my other matrix as well
	}
	return Camera::getViewMatrix();
}

Matrix SphericalCamera::getMatrixXPlus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookXPlus;
}
Matrix SphericalCamera::getMatrixXMinus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookXMinus;
}

Matrix SphericalCamera::getMatrixYPlus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookYPlus;
}
Matrix SphericalCamera::getMatrixYMinus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookYMinus;
}

Matrix SphericalCamera::getMatrixZPlus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookZPlus;
}
Matrix SphericalCamera::getMatrixZMinus()
{
	if (m_isDirty_matView) {
		updatePointLightViewMatrixs();
		m_isDirty_matView = false;
		//update my other matrix as well
	}
	return m_matLookZMinus;
}