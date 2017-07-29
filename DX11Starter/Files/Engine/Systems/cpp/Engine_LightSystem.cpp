#include <Engine\Systems\LightSystem.h>

using namespace KEngine;
using namespace KEngine::KSystem;

const float LightSystem::RADIUS_PER_LIGHT_INTENSITY = 15;

void LightSystem::addEntityHandle(Entity & entity, LightEntity & componenet)
{
	componenet.lightType = LightType::PointLight;
	componenet.index = m_pointLights.size();
	m_pointLights.push_back({ entity.transform3D->position,Vector3(1,1,1),1.0f});
}

void LightSystem::setPointLight(int index, Vector3 color, float intensity)
{
	if (m_components[index].lightType == LightType::PointLight) {
		m_pointLights[m_components[index].index].color = color;
		m_pointLights[m_components[index].index].intensity = intensity;
		return;
	}
	//remove from spotlight
	//then add to pointlight
}

void LightSystem::setSpotLight(int index, Vector3 color, float intensity, float cornRadius)
{
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
