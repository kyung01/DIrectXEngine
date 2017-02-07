#include <Game\ContextExamples.h>
#include <Game\Script\Y_Axis_Rotate.h>
#include <Game\Script\X_Axis_Rotate.h>
using namespace NGame;

void NGame::LoadExample00(Context &context)
{
	
	if(false)for (int i = 0; i < 10; i++) for (int j = 0; j < 5; j++)
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_TORUS;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_PROPERTY] = NGraphic::KEnum::TEXTURE_ID_RED;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->setScale(Vector3(0.7, 0.7, 0.7));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(-5.0f + i, -2.5 + j, 6);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), rand() * 100) *Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), rand() * 100)*Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), rand() * 100));
	}
	if (false)for (int i = 0; i < 10; i++) for (int j = 0; j < 5; j++)
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_TORUS;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_PROPERTY] = NGraphic::KEnum::TEXTURE_ID_RED;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->setScale(Vector3(0.7, 0.7, 0.7));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(-5.0f + i, -2.5 + j, 9);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), rand() * 100) *Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), rand() * 100)*Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), rand() * 100));
	}
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		obj.get()->setScale(Vector3(100, 100, 100));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(0, 0, 0);
	}
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		obj.get()->setScale(Vector3(100, 100, 100));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(0, 0,10);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), 3.14));
	}
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		obj.get()->setScale(Vector3(100, 100, 100));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(0, -10, 0);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14 / 2));
	}
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		obj.get()->setScale(Vector3(100, 100, 100));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(10, 0, 0);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), -3.14 / 2));
	}
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		obj.get()->setScale(Vector3(100, 100, 100));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(-10, 0, 0);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), 3.14 / 2));
	}
	





	{
		auto lightObj = context.m_scene->getSpotLight(3.14/2, Vector4(1,0,0,20),1);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(-2.0, 0, 1.5);
		lightObj->m_lightColor = Vector4(1,0,0,20);
		lightObj->setScale(Vector3(15, 15, 15));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));

	}
	{
		auto lightObj = context.m_scene->getSpotLight(3.14 / 2, Vector4(0, 1, 0, 20), 1);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(0, 3, 1.5);
		lightObj->m_lightColor = Vector4(0, 1, 0,20);
		lightObj->setScale(Vector3(15, 15, 15));
		lightObj->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 3.14 / 5));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));
	}
	{
		auto lightObj = context.m_scene->getSpotLight(3.14 / 2, Vector4(0, 0, 1, 20), 1);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(2, 0, 1.5);
		lightObj->m_lightColor = Vector4(0, 0, 1, 20);
		lightObj->setScale(Vector3(15, 15, 15));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));
	}


}
