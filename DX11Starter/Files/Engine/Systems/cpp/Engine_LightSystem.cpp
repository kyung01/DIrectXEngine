#include <Engine\Systems\LightSystem.h>

using namespace KEngine;
using namespace KEngine::KSystem;
void LightSystem::addEntityHandle(Entity & entity, LightEntity & componenet)
{
	componenet.lightType = LightType::PointLight;
	componenet.index = m_pointLights.size();
	m_pointLights.push_back({Vector3(1,1,1),1.0f});
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
