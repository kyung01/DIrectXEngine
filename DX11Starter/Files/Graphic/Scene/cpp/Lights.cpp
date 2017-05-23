#include <Graphic\Scene\PointLight.h>
#include <Graphic\Scene\SpotLight.h>
using namespace NGraphic::NScene;
PointLight::PointLight()
{
	m_lightType=LIGHT_TYPE::POINTLIGHT;
}

SpotLight::SpotLight()
{
	m_lightType = LIGHT_TYPE::SPOTLIGHT;
}
