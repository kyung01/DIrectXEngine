#include <Engine\Systems\LightSystem.h>
#include <DirectX\DirectXUtility.h>
using namespace KEngine;
using namespace KEngine::KSystem;
/*

struct PointLightInfo {
Vector3 position;
Vector3 color;
float intensity;
};
struct SpotLightInfo {
Vector3 position;
Quaternion rotation;
Vector3 color;
float intensity;
float fov;
};
*/
const float LightSystem::RADIUS_PER_LIGHT_INTENSITY = 15;

void LightSystem::addEntityHandle(Entity & entity, LightComponent & componenet)
{
	Vector3 randomColor(DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM());
	float biggestRGB = max(randomColor.x, max(randomColor.y, randomColor.z) );
	randomColor *= 1 / biggestRGB;
	float lightIntensity = (DirectX::DirectXUtility::GET_RANDOM() % 100)/100.0f * 3.0f;
	float lightFOV = (DirectX::DirectXUtility::GET_RANDOM() % 100) / 100.0f * 3.14f;
	Vector3 randomRotationAngle(DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM());
	randomRotationAngle.Normalize();
	Quaternion lightRotation = Quaternion::CreateFromAxisAngle(randomRotationAngle, DirectX::DirectXUtility::GET_RANDOM()/100.0f);

	if (DirectX::DirectXUtility::GET_RANDOM() % 2 == 0) {
		//add point light
		componenet.lightType = LightType::PointLight;
		componenet.lightIndex = m_pointLights.size();
		m_pointLights.push_back({ entity.m_transform3D->position,randomColor,lightIntensity });
	}
	else {
		//otherwise add spot light
		componenet.lightType = LightType::SpotLight;
		componenet.lightIndex = m_spotLights.size();
		m_spotLights.push_back({ entity.m_transform3D->position,lightRotation,randomColor,lightIntensity ,lightFOV });
	}

}

void LightSystem::addEntityLinkRecreate(Entity & entity, LightComponent & componenet)
{
	entity.m_lightComponent = &componenet;
}

LightSystem::LightSystem()
{
	m_isFrustumNeedUpdate = true;
}

void KEngine::KSystem::LightSystem::init(float widthOverHeight, float nearDistance, float farDistance, int divisionX, int divisionY, int divisionZ)
{
	m_frustum.init(widthOverHeight,nearDistance,farDistance,divisionX,divisionY,divisionZ);
}

void LightSystem::update(std::vector<Entity>& entities, float time)
{
	for (int i = 0; i < m_components.size(); i++) {
		if (entities[m_components[i].entityIndex].m_transform3D->isDirty) {
			m_isFrustumNeedUpdate = true;
			if (m_components[i].lightType == LightType::PointLight) {
				m_pointLights[m_components[i].lightIndex].position = entities[m_components[i].entityIndex].m_transform3D->position;
			}
			else {
				m_spotLights[m_components[i].lightIndex].position = entities[m_components[i].entityIndex].m_transform3D->position;
			}
		}
	}
	if (!m_isFrustumNeedUpdate) return;

	m_frustum.testBegin();
	for (int i = 0; i < m_components.size(); i++) {
		int lightIndex = i;
		if (m_components[i].lightType == LightType::PointLight) {
			auto& pointLight = m_pointLights[m_components[i].lightIndex];
			m_frustum.testPointlight(lightIndex, pointLight.position, pointLight.intensity * RADIUS_PER_LIGHT_INTENSITY);
		}
		else {
			auto& spotLight = m_spotLights[m_components[i].lightIndex];
			Vector3 directionLook( DirectX::XMVector3Rotate(Vector3(0, 0, 1), spotLight.rotation ) ) ;
			m_frustum.testSpotlight(lightIndex, spotLight.position, directionLook,spotLight.intensity * RADIUS_PER_LIGHT_INTENSITY, spotLight.fov);
		}
	}
	m_isFrustumNeedUpdate = false;
}

void LightSystem::setPointLight(int index, Vector3 color, float intensity)
{
	if (m_components[index].lightType == LightType::PointLight) {
		m_pointLights[m_components[index].lightIndex].color = color;
		m_pointLights[m_components[index].lightIndex].intensity = intensity;
		return;
	}
	//remove from spotlight
	//then add to pointlight
}

void LightSystem::setSpotLight(int index, Vector3 color, float intensity, float cornRadius)
{
}

int KEngine::KSystem::LightSystem::getLightCount()
{
	return m_components.size();
}

LightType KEngine::KSystem::LightSystem::getLightType(int n)
{
	return m_components[n].lightType;
}

PointLightInfo KEngine::KSystem::LightSystem::getPointLight(int n)
{
	return m_pointLights[n];
}

SpotLightInfo KEngine::KSystem::LightSystem::getSpotLight(int n)
{
	return m_spotLights[n];
}

void LightSystem::run()
{
	m_frustum.testBegin();
	for (int i = 0; i < m_pointLights.size(); i++) {

		m_frustum.testPointlight(i, m_pointLights[i].position, m_pointLights[i].intensity * RADIUS_PER_LIGHT_INTENSITY);
	}
	for (int i = 0; i < m_spotLights.size(); i++) {

		m_frustum.testSpotlight(i, m_spotLights[i].position, Vector3::Forward, m_spotLights[i].intensity * RADIUS_PER_LIGHT_INTENSITY, m_spotLights[i].fov);
	}
}
