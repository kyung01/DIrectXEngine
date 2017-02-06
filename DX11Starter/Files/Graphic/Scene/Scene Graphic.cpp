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
	obj->m_ObjectType = KEnum::OBJ_TYPE_UI;
	objs_ui.push_back(obj);
	return obj;
}

std::shared_ptr<Light> NGraphic::NScene::Scene::getObjLight()
{
	auto obj = std::shared_ptr<Light>(new Light());
	objs_lights.push_back(obj);
	return obj;
}
