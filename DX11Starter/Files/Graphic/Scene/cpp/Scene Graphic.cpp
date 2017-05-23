#include <Graphic\Scene\Scene.h>
using namespace NGraphic;
using namespace NGraphic::NScene;

const int ATLAS_TEXTURE_WIDTH_COUNT = 10;
const int ATLAS_TEXTURE_HEIGHT_COUNT = 10;
Scene::Scene():
	size(10,10,10)
{
	//m_lightAtlasSheet = std::shared_ptr<RenderTexture>(new RenderTexture());
	//m_lightAtlasSheet->init(device, atlasTextureWidth, atlasTextureHeight);
	//
	//m_atlasSlicer = std::make_shared<TextureAtlasSlicer>(
	//	atlasTextureWidth, atlasTextureHeight,
	//	ATLAS_TEXTURE_WIDTH_COUNT, ATLAS_TEXTURE_HEIGHT_COUNT);
}


void NScene::Scene::addObject(std::shared_ptr<Object> obj)
{
}

void Scene::loadExample00()
{
}

std::shared_ptr<Object> NGraphic::NScene::Scene::getObjSolid()
{
	auto obj = std::shared_ptr<Object>(new Object());
	objs_solid.push_back(obj);
	return obj;
}

std::shared_ptr<Object> NGraphic::NScene::Scene::getObjUI()
{
	auto obj = std::shared_ptr<Object>(new Object());
	objs_ui.push_back(obj);
	return obj;
}

std::shared_ptr<OldPointLight> NGraphic::NScene::Scene::getPointLightOLD(
	Vector3 lightColor, float lightDistance)
{
	std::shared_ptr<OldPointLight> light = std::make_shared<OldPointLight>();
	light->m_lightType = LIGHT_TYPE::POINTLIGHT;
	light->setLightColor( lightColor);
	light->m_lightDistance = lightDistance;
	//objs_lightsNotReady.push_back(light);
	objs_lightsNotReady.push_back(light);
	{
	}
	return light;
}

std::shared_ptr<OldSpotLight> Scene::getSpotLightOLD(
	float angle, Vector3 lightColor, float lightDistance)
{
	std::shared_ptr<OldSpotLight> light = std::make_shared<OldSpotLight>();
	light->m_lightType = LIGHT_TYPE::SPOTLIGHT;
	light->setLightColor( lightColor);
	light->setFOV(angle);
	light->m_lightDistance = lightDistance;
	//objs_lightsNotReady.push_back(light);
	objs_lightsNotReady.push_back(light);
	

	return light;
}

std::shared_ptr<Probe> NGraphic::NScene::Scene::getProbeReflection()
{
	std::shared_ptr<Probe> probe = std::make_shared<Probe>();
	m_probesNotReady.push_back(probe);
	{

	}
	return probe;
}

std::shared_ptr<Probe> NGraphic::NScene::Scene::getProbeDiffuse()
{
	return std::shared_ptr<Probe>();
}

PointLight NGraphic::NScene::Scene::getPointLight(Vector3 lightColor, float lightDistance)
{

	PointLight light;
	light.setLightColor(lightColor);
	light.m_lightDistance = lightDistance;
	m_lightsNotReady.push_back(light);
	return light;
}

SpotLight NGraphic::NScene::Scene::getSpotLight(float angle, Vector3 lightColor, float lightDistance)
{
	SpotLight light;
	light.setLightColor(lightColor);
	light.setFOV(angle);
	light.m_lightDistance = lightDistance;
	m_lightsNotReady.push_back(light);

	return light;
}
