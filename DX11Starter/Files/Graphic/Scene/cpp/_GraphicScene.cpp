#include <Graphic\Scene\Object.h>
#include <Graphic\Scene\SpotLight.h>
#include <Graphic\Scene\PointLight.h>
#include <Graphic\Scene\ILight.h>
#include <Graphic\Scene\Camera.h>
#include <Graphic\Scene\SphericalCamera.h>

using namespace NGraphic;
using namespace NGraphic::NScene;

int Object::OBJECT_UNIQUE_ID = 0;

Object::Object() :
	m_isAlive(true),
	m_id(OBJECT_UNIQUE_ID++),
	m_ObjectType(OBJ_TYPE::SOLID),
	m_meshId(KEnum::MESH_ID_CUBE),
	m_renderType(KEnum::RENDER_DEFAULT),
	m_isDirty(true),
	m_scale(1, 1, 1),
	m_dirLook(0, 0, 1)
{
	m_textures[TEXTURE_TYPE_DIFFUSE] = TEXTURE_ID_WHITE;
	m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_DEFAULT;
	m_textures[TEXTURE_TYPE_SPECULAR] = TEXTURE_ID_HEIGHT_DEFAULT;
	m_textures[TEXTURE_TYPE_DISPLACEMENT] = TEXTURE_ID_DEFAULT;
}

Object Object::setPos(Vector3 pos)
{
	m_isDirty = true;
	m_pos = pos;
	return *this;
}
Object Object::setScale(Vector3 scale)
{
	m_isDirty = true;
	m_scale = scale;
	return *this;
}
Object Object::setRotation(Quaternion quaternion)
{
	m_isDirty = true;
	m_rotation = quaternion;
	m_dirLook = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_rotation);
	return *this;
}


Matrix Object::getModelMatrix()
{
	if (m_isDirty) {
		m_isDirty = false;
		m_matModel =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)*
			DirectX::XMMatrixRotationQuaternion(m_rotation)*
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	}
	return m_matModel;
}
//Camera
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

void NGraphic::NScene::Camera::setFOV(float value)
{
	m_isDirty_matProjection = true;
	m_fov = value;
}

Object Camera::setPos(Vector3 pos)
{
	Object::setPos(pos);
	m_isDirty_matView = true;
	return *this;
}


Object Camera::setRotation(Quaternion quaternion)
{
	Object::setRotation(quaternion);
	m_isDirty_matView = true;
	return *this;

}

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

NGraphic::NScene::SphericalCamera::SphericalCamera()
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

//Light

ILight::ILight()
{
	//The light model bases 26.56 degree or 0.463 radiance as the base
	m_lightType = LIGHT_TYPE::POINTLIGHT;
	m_lightColor = Vector3(1, 1, 1);
}


void ILight::setLightColor(Vector3 color)
{
	m_isLightDirty = true;
	m_lightColor = color;
}

void ILight::setLightColor(float r, float g, float b)
{
	m_isLightDirty = true;
	m_lightColor = Vector3(r, g, b);
}

Vector3 ILight::getLightColor()
{
	return m_lightColor;
}



SpotLight::SpotLight()
{
	//The light model bases 26.56 degree or 0.463 radiance as the base
	Camera::Camera();
	m_lightType = LIGHT_TYPE::POINTLIGHT;
	m_ObjectType = OBJ_TYPE::LIGHT;
	m_fov = 3.14f * 0.5;// 26.56 / (360 / 6.28318530718);
	m_lightColor = Vector3(1, 1, 1);
	setScale(Vector3(1, 1, 1));
}

/*

void SpotLight::setLightColor(Vector3 color)
{
	m_isLightDirty = true;
	m_lightColor = color;
}

void SpotLight::setLightColor(float r, float g, float b)
{
	m_isLightDirty = true;
	m_lightColor = Vector3(r, g, b);
}

Vector3 SpotLight::getLightColor()
{
	return m_lightColor;
}
*/


NGraphic::NScene::PointLight::PointLight()
{
	SpotLight();
	m_lightType = LIGHT_TYPE::POINTLIGHT;
	setFOV(3.14 / 2 + 0.15f);
}


