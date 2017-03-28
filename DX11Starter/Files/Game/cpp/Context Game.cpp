#include <Game\Context.h>
#include <iostream>
using namespace NGame;
NGame::Context::Context()
{
}
float cosMag = 0;
void NGame::Context::update(float timeElapsed)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); it++) {
		(*it)->update(*this, timeElapsed);
	}

	for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
		(*it)->update(*this, timeElapsed);
	}
	//return;

	cosMag += timeElapsed*0.01;

	Vector3 pos = Vector3(0, 2, 0) + Vector3(3.5*cos(cosMag), sin(cosMag) * 2, 0);
	solidLightPoint->setPos(pos.x, pos.y, pos.z);
	return;
	{
		float angleX = cosMag*1.1;
		float angleY = cosMag*1.5;
		float angleZ = cosMag*1.23;

		Vector3 pos = Vector3(0, 0, 5) + Vector3(-cos(cosMag)*10, -sin(cosMag) * 2, 0);
		solidLightSpot->setPos(pos.x, pos.y, pos.z);

		solidLightSpot->setRotation(solidLightSpot->m_rot
			* Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), timeElapsed*.1)
			* Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), timeElapsed*.07)
			* Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), timeElapsed*.05)
		);
		
		solidLightSpot->m_graphicObjects.begin()->get()->setRotation(
			DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14 / 2) *solidLightSpot->m_rot);
	}




}
void Context::init(NGraphic::NScene::Scene * scene)
{
	m_scene = scene;
	//return;

	auto sphereLight = scene->getPointLight(Vector3(1, 1, 1), 6.0f);
	auto objSolidPointLight = Light::GET_POINTLIGHT(Vector4(1, 1, 1, 1),0.5f);
	objSolidPointLight->m_graphicObjects.push_back(sphereLight);
	objSolidPointLight->setPos(0, 0, 5);
	m_lights.push_back(objSolidPointLight);
	this->solidLightPoint = objSolidPointLight;
	return;

	//auto cone = scene->getObjSolid();
	auto coneLight = scene->getSpotLight(3.14/2,Vector3(1,1,1),1);
	//cone->m_meshId = NGraphic::MESH_ID_CONE;
	//cone->setScale(Vector3(2.0f));
	//cone->setRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14 / 2));

	auto objSolidSpotLight = Light::GET_SPOTLIGHT(3.14159265359 /2.0f,Vector4(1, 1, 1, 1), 1.0f);
	this->solidLightSpot = objSolidSpotLight;
	//objSolidPointLight->m_graphicObjects.push_back(sphere00);
	
	//objSolidSpotLight->m_graphicObjects.push_back(cone);
	objSolidSpotLight->m_graphicObjects.push_back(coneLight);
	objSolidSpotLight->setPos(0, 0, 5);

	m_lights.push_back(objSolidSpotLight);


}
void Context::addEntity(std::shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}
