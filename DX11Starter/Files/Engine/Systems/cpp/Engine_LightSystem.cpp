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
const float LightSystem::RADIUS_PER_LIGHT_INTENSITY = 100;

void LightSystem::addEntityHandle(Entity & entity, LightComponent & componenet)
{
	//Vector3 randomColor(DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM(), DirectX::DirectXUtility::GET_RANDOM());
	Vector3 randomColor(rand(), rand(), rand());
	float biggestRGB = max(randomColor.x, max(randomColor.y, randomColor.z) );
	randomColor *= 1 / biggestRGB;
	std::cout << "Random_light_color " << randomColor.x << " " << randomColor.y << " " << randomColor.z << std::endl;

	float lightIntensity = 1+ (DirectX::DirectXUtility::GET_RANDOM() % 100)/100.0f * 3.0f;
	float lightFOV = 3.14f / 2.0f;
	Quaternion lightRotation;

	if (DirectX::DirectXUtility::GET_RANDOM() % 2 == 0) {
	//if (true) {
		//add point light
		componenet.lightType = LIGHT_TYPE::POINT_LIGHT;
		componenet.lightIndex = m_pointLights.size();
		m_pointLights.push_back({ entity.m_transform3D->position,randomColor,lightIntensity });
	}
	else {
		//otherwise add spot light
		componenet.lightType = LIGHT_TYPE::SPOT_LIGHT;
		componenet.lightIndex = m_spotLights.size();
		m_spotLights.push_back({ entity.m_transform3D->position,lightRotation,randomColor,lightIntensity ,lightFOV });
	}

}

void LightSystem::addEntityLinkRecreate(std::vector<Entity> & entityVectors, LightComponent & componenet)
{
	entityVectors[componenet.entityIndex].m_lightComponent = &componenet;
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
		Entity& entity = entities[m_components[i].entityIndex];
		if (entity.m_transform3D->isDirty) {
			m_isFrustumNeedUpdate = true;
			if (m_components[i].lightType == LIGHT_TYPE::POINT_LIGHT) {
				m_pointLights[m_components[i].lightIndex].position = entity.m_transform3D->position;
			}
			else {
				m_spotLights[m_components[i].lightIndex].position = entity.m_transform3D->position;
				m_spotLights[m_components[i].lightIndex].rotation = entity.m_transform3D->rotation;
			}
		}
	}
	if (!m_isFrustumNeedUpdate) return;

	m_frustum.testBegin();
	for (int i = 0; i < m_components.size(); i++) {
		int lightIndex = i;
		if (m_components[i].lightType == LIGHT_TYPE::POINT_LIGHT) {
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
	if (m_components[index].lightType == LIGHT_TYPE::POINT_LIGHT) {
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

LIGHT_TYPE KEngine::KSystem::LightSystem::getLightType(int n)
{
	return m_components[n].lightType;
}

PointLightInfo KEngine::KSystem::LightSystem::getPointLight(int n)
{
	
	return m_pointLights[m_components[n].lightIndex];
}

SpotLightInfo KEngine::KSystem::LightSystem::getSpotLight(int n)
{
	return m_spotLights[m_components[n].lightIndex];
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
