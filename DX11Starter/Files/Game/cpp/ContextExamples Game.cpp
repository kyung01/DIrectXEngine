#include <Game\ContextExamples.h>
#include <Game\Script\Y_Axis_Rotate.h>
#include <Game\Script\X_Axis_Rotate.h>
using namespace NGame;

void NGame::LoadExample00(Context &context)
{

	auto plazaSolidObject = context.m_scene->getObjSolid();
	plazaSolidObject.get()->m_meshId = NGraphic::MESH_ID_SPONZA;
	//if (i <= 5)
	//if (j <= 2)
	//	obj.get()->m_meshId = NGraphic::MESH_ID_CUBE;
	plazaSolidObject.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
	plazaSolidObject.get()->m_textures[NGraphic::TEXTURE_TYPE_PROPERTY] = NGraphic::KEnum::TEXTURE_ID_RED;
	plazaSolidObject.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
	plazaSolidObject.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = NGraphic::KEnum::TEXTURE_ID_WHITE;
	plazaSolidObject.get()->setScale(Vector3(0.005, 0.005, 0.005));
	
	//if (j <= 2)
	//	obj.get()->setScale(Vector3(0.5, 0.5, 0.5));
	auto entitySponza = new Entity();
	context.addEntity(std::shared_ptr<Entity>(entitySponza));
	entitySponza->m_graphicObjects.push_back(plazaSolidObject);
	entitySponza->setPos(0,0,0);
	//entitySponza->setRotation(Quaternion::Identity);
	
	if (false)for (int i = 0; i < 10; i++) for (int j = 0; j < 5; j++)
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_PLANE;
		//if (i <= 5)
		//if (j <= 2)
		//	obj.get()->m_meshId = NGraphic::MESH_ID_CUBE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_PROPERTY] = NGraphic::KEnum::TEXTURE_ID_RED;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->setScale(Vector3(0.7, 0.7, 0.7));
		if (i <= 5){
			obj.get()->m_meshId = NGraphic::MESH_ID_HELIX;
			obj.get()->setScale(Vector3(0.2, 0.2, 0.2));
		}
		//if (j <= 2)
		//	obj.get()->setScale(Vector3(0.5, 0.5, 0.5));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(-5.0f + i, 0 ,  + j);
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
	if (false) {
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
			e->setPos(0, 0, 10);
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
	}
	
	
	





	{
		auto lightObj = context.m_scene->getSpotLight(3.14/2, Vector3(20,0,0),10000);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(0.0, 0, 0.0);
		lightObj->setLightColor(5,0,0);
		//lightObj->setScale(Vector3(15, 15, 15));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));

	}
	//return;
	{
		auto lightObj = context.m_scene->getSpotLight(3.14 / 2, Vector3(0, 0, 20), 10000);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(2, 0, 1.5);
		lightObj->setLightColor(0, 0, 5);
		lightObj->setScale(Vector3(15, 15, 15));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));
	}
	//return;
	{
		auto lightObj = context.m_scene->getSpotLight(3.14 / 2, Vector3(0, 20, 0), 10000);
		auto lightEntity = new Entity();
		lightEntity->m_graphicObjects.push_back(lightObj);
		lightEntity->setPos(0, 3, 1.5);
		lightObj->setLightColor(0, 5, 0);
		lightObj->setScale(Vector3(15, 15, 15));
		lightObj->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 3.14 / 5));
		context.addEntity(std::shared_ptr<Entity>(lightEntity));
	}
	


}
