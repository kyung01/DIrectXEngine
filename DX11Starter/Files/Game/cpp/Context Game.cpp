#include <Game\Context.h>
#include <iostream>
using namespace NGame;
NGame::Context::Context()
{
}
float cosMag = 0;
void NGame::Context::update(float timeElapsed)
{
	cosMag += timeElapsed;
	if (debugSphere) {
		Vector3 pos = Vector3(0, 0, 5) + Vector3(cos(cosMag*0.1) * 3, 0, 0);
		debugSphere->setPos(pos.x, pos.y, pos.z);
		//frustum.test(result, frustum.planesX, debugSphere->position, 0.2);
		frustum.testPointlight( debugSphere->position, 0.2);
	}
	std::cout << "erer\n";
	for (auto it = m_entities.begin(); it != m_entities.end(); it++) {
		(*it)->update(*this, timeElapsed);
	}
}
void Context::init(NGraphic::NScene::Scene * scene)
{
	m_scene = scene;
	auto sphere00 = scene->getObjSolid();
	sphere00->m_meshId = NGraphic::MESH_ID_SPHERE;
	sphere00->setScale(Vector3(0.2f));
	std::shared_ptr<Entity> testSphere = std::make_shared<Entity>();
	debugSphere =&*testSphere;
	testSphere->m_graphicObjects.push_back(sphere00);
	
	testSphere->setPos(0, 0, 5);
	addEntity(testSphere);

	frustum.init(3.14 / 4,1.0f,100.f, 5,5,5);

}
void Context::addEntity(std::shared_ptr<Entity> entity) {
	m_entities.push_back(entity);
}
