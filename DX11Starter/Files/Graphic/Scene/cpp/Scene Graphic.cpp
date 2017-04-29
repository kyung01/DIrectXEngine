#include "Graphic\Scene\Scene.h"
#include "Graphic\Scene\Light.h"
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

std::shared_ptr<PointLight> NGraphic::NScene::Scene::getPointLight(
	Vector3 lightColor, float lightDistance)
{
	std::shared_ptr<PointLight> light = std::make_shared<PointLight>();
	//light->m_lightType = LIGHT_TYPE::POINTLIGHT;
	light->setLightColor( lightColor);
	light->m_lightDistance = lightDistance;
	//objs_lightsNotReady.push_back(light);
	objs_lightsNotReady.push_back(light);

	return light;
}

std::shared_ptr<Light> NGraphic::NScene::Scene::getSpotLight(
	float angle, Vector3 lightColor, float lightDistance)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->m_lightType = LIGHT_TYPE::SPOTLIGHT;
	light->setLightColor( lightColor);
	light->setFOV(angle);
	light->m_lightDistance = lightDistance;
	//objs_lightsNotReady.push_back(light);
	objs_lightsNotReady.push_back(light);

	return light;
}
