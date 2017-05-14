#include <Game\Context.h>
#include <iostream>
using namespace NGame;
float NGame::Context::hprRandomFloat()
{
	return (rand()%100)/100.0f;
}
Vector3 NGame::Context::hprGetRandomColor(int seed)
{
	Vector3 colors[] = { Vector3(1,0,0) ,Vector3(1,1,0) ,Vector3(0,1,0),Vector3(0,1,1),Vector3(0,0,1),Vector3(1,0,1) };
	int index = (seed*100) % 6;
	float ratio = ((seed * 357) % 100) / 100.0f;
	Vector3 colorFrom = colors[index];
	Vector3 colorTo = colors[(index+1)%6];
	Vector3 color = colorFrom + (colorTo - colorFrom)*ratio;
	std::cout <<"SED "<< seed << " RATIO " << ratio <<" COLOR " << color.x << " " << color.y << " " << color.z << "\n";
	return colorFrom + (colorTo - colorFrom)*ratio;
}
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
	return;

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
//
void Context::init(NGraphic::NScene::Scene * scene)
{
	m_scene = scene;
	return;
	int lightType = 1;
	float X_MAX = 5.0f, Y_MAX = 3.0f, Z_MAX = 2.0f;
	//return;
	for (int i = 0; i < 20 ; i++) {
		Vector3 lightPosition(-5 + X_MAX *	 hprRandomFloat(),2+ Y_MAX * hprRandomFloat(), -1+Z_MAX * hprRandomFloat());
		Vector3 lightColor = hprGetRandomColor(hprRandomFloat() * 100);
		//auto objSolidPointLight = Light::GET_POINTLIGHT(Vector4(lightColor.x, lightColor.y, lightColor.z, 1),5.0f);
		float e = hprRandomFloat();

		Vector3 randAxis(rand(), rand(), rand());
		randAxis.Normalize();

		if (lightType++%2 == 0) {
			auto gamePointLight = Light::GET_POINTLIGHT(Vector4(lightColor.x, lightColor.y, lightColor.z, 1),0.1f);
			auto sphereLight = scene->getPointLight(lightColor, 13.0f);
			gamePointLight->m_graphicObjects.push_back(sphereLight);
			m_lights.push_back(gamePointLight);
			
			gamePointLight->setRotation(Quaternion::CreateFromAxisAngle(randAxis, rand() )  );
			gamePointLight->setPos(lightPosition.x, lightPosition.y, lightPosition.z);

		}
		else {
			auto gameSpotLight = Light::GET_SPOTLIGHT(3.14f/2,Vector4(lightColor.x, lightColor.y, lightColor.z, 1), 0.1f);
			auto spotLight = scene->getSpotLight(3.14f/2,lightColor, 13);
			spotLight->setLightColor(lightColor);
			gameSpotLight->m_graphicObjects.push_back(spotLight);
			m_lights.push_back(gameSpotLight);
			
			gameSpotLight->setRotation(Quaternion::CreateFromAxisAngle(randAxis, rand()));
			gameSpotLight->setPos(lightPosition.x, lightPosition.y, lightPosition.z);
		}
	}
	//auto sphereLight = scene->getPointLight(Vector3(1, 1, 1), 5.0f);
	//auto objSolidPointLight = Light::GET_POINTLIGHT(Vector4(1, 1, 1, 1), 5.0f);
	//objSolidPointLight->m_graphicObjects.push_back(sphereLight);
	//objSolidPointLight->setPos(0, 0, 5);
	//m_lights.push_back(objSolidPointLight);
	//this->solidLightPoint = objSolidPointLight;
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

int lightType = 0;
void NGame::Context::addLight(NGraphic::NScene::Scene & scene)
{
	float X_MAX = 5.0f, Y_MAX = 10.0f, Z_MAX = 5.0f;

	//Vector3 lightPosition(-X_MAX + (2 * X_MAX) *	 hprRandomFloat(), Y_MAX * hprRandomFloat(), -Z_MAX + 2 * Z_MAX * hprRandomFloat());
	Vector3 lightPosition(scene.m_camMain.m_pos );
	//Vector3 lightColor = hprGetRandomColor(hprRandomFloat() * 100);
	Vector3 lightColor(0.5f, 0.5f, 1.0f);
	lightType = 0;
	if (lightType++ % 2 == 0) {
		auto gamePointLight = Light::GET_POINTLIGHT(Vector4(lightColor.x, lightColor.y, lightColor.z, 1), 13.1f);
		auto sphereLight = scene.getPointLight(lightColor, 13.0f);
		gamePointLight->m_graphicObjects.push_back(sphereLight);
		m_lights.push_back(gamePointLight);

		//gamePointLight->setRotation(scene.m_camMain.m_rotation);
		gamePointLight->setPos(lightPosition.x, lightPosition.y, lightPosition.z);

	}
	else {
		auto gameSpotLight = Light::GET_SPOTLIGHT(3.14f / 2, Vector4(lightColor.x, lightColor.y, lightColor.z, 1), 13.1f);
		auto spotLight = scene.getSpotLight(3.14f / 2, lightColor, 13);
		spotLight->setLightColor(lightColor);
		gameSpotLight->m_graphicObjects.push_back(spotLight);
		m_lights.push_back(gameSpotLight);

		//gameSpotLight->setRotation(scene.m_camMain.m_rotation);
		gameSpotLight->setPos(lightPosition.x, lightPosition.y, lightPosition.z);
	}
}

void NGame::Context::addProbe(NGraphic::NScene::Scene & scene)
{

	float X_MAX = 5.0f, Y_MAX = 10.0f, Z_MAX = 5.0f;

	Vector3 lightPosition(-X_MAX + (2 * X_MAX) *	 hprRandomFloat(),2+ Y_MAX * hprRandomFloat(), -Z_MAX + 2 * Z_MAX * hprRandomFloat());
	Vector3 randAxis(rand(), rand(), rand());
	randAxis.Normalize();
	//auto probe = scene.getProbe();
	auto probeGame = std::make_shared<Probe>();
	probeGame->m_graphicProbe = scene.getProbe();
	probeGame->m_graphicObjects.push_back(probeGame->m_graphicProbe);
	m_probes.push_back(probeGame);

	probeGame->setRotation(Quaternion::CreateFromAxisAngle(randAxis, rand()));
	probeGame->setPos(lightPosition.x, lightPosition.y, lightPosition.z);
	
}
