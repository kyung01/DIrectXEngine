#include "Graphic\Scene\Scene.h"
#include "Graphic\Scene\Light.h"
using namespace NGraphic;
using namespace NGraphic::NScene;
Scene::Scene():
	size(10,10,10)
{
}

Scene::Scene(float x, float y, float z):size(x,y,z)
{
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

std::shared_ptr<Light> NGraphic::NScene::Scene::getPointLight(Vector4 lightColor, float lightDistance)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->m_lightType = LIGHT_TYPE::POINTLIGHT;
	light->m_lightColor = lightColor;
	light->m_lightDistance = lightDistance;
	objs_lights.push_back(light);

	return light;
}

std::shared_ptr<Light> NGraphic::NScene::Scene::getSpotLight(float angle, Vector4 lightColor, float lightDistance)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->m_lightType = LIGHT_TYPE::SPOTLIGHT;
	light->m_lightColor = lightColor;
	light->setFOV(angle);
	light->m_lightDistance = lightDistance;
	objs_lights.push_back(light);

	return light;
}
