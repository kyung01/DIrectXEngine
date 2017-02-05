#include <Game\Context.h>
#include <iostream>
using namespace NGame;
NGame::Context::Context()
{
}
float cosMag = 0;
void NGame::Context::update(float timeElapsed)
{
	cosMag += timeElapsed*0.15;

	Vector3 pos = Vector3(0, 0, 5) + Vector3(cos(cosMag), sin(cosMag) * 2, 0);
	lightPoint->setPos(pos.x, pos.y, pos.z);
	{
		float angleX = cosMag*1.1;
		float angleY = cosMag*1.5;
		float angleZ = cosMag*1.23;

		Vector3 pos = Vector3(0, 0, 5) + Vector3(-cos(cosMag)*0.5, -sin(cosMag) * 2, 0);
		lightSpot->setPos(pos.x, pos.y, pos.z);

		lightSpot->setRotation(lightSpot->m_rot
			* Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), timeElapsed*1.1)
			* Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), timeElapsed*2.37)
			* Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), timeElapsed*3.25)
		);
		lightSpot->m_graphicObjects.begin()->get()->setRotation(
			DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14 / 2) *lightSpot->m_rot);
	}




	frustum.testBegin();
	frustum.testPointlight(lightPoint->m_pos, 1.0);
	frustum.testSpotlight(lightSpot->m_pos, lightSpot->m_look, lightSpot->distance,lightSpot->angle);
	for (auto it = m_entities.begin(); it != m_entities.end(); it++) {
		(*it)->update(*this, timeElapsed);
	}

	for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
		(*it)->update(*this, timeElapsed);
	}
}
void Context::init(NGraphic::NScene::Scene * scene)
{
	m_scene = scene;
	auto sphere00 = scene->getObjSolid();
	sphere00->m_meshId = NGraphic::MESH_ID_SPHERE;
	sphere00->setScale(Vector3(2.0f));
	auto cone = scene->getObjSolid();
	cone->m_meshId = NGraphic::MESH_ID_CONE;
	cone->setScale(Vector3(2.0f));
	cone->setRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14 / 2));

	auto objPointLight = Light::GET_POINTLIGHT(Vector4(1, 1, 1, 1), 0.5f);
	lightPoint = objPointLight;
	auto objSpotLight = Light::GET_SPOTLIGHT(3.14159265359 /2.0f,Vector4(1, 1, 1, 1), 1.0f);
	lightSpot = objSpotLight;
	objPointLight->m_graphicObjects.push_back(sphere00);
	objPointLight->setPos(0, 0, 5);
	objSpotLight->m_graphicObjects.push_back(cone);
	objSpotLight->setPos(0, 0, 5);

	m_lights.push_back(objPointLight);
	m_lights.push_back(objSpotLight);

	frustum.init(3.14 / 4,0.1f,15.f, 8,8,10);

}
void Context::addEntity(std::shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}
