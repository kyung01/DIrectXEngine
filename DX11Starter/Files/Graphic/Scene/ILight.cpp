#include "ILight.h"
using namespace NGraphic::NScene;

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
