#include <Game\ContextExamples.h>
#include <Game\Script\Y_Axis_Rotate.h>
#include <Game\Script\X_Axis_Rotate.h>
using namespace NGame;

void NGame::LoadExample00(Context &context)
{
	for (int i = 0; i < 6; i++) {
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_SPHERE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_PROPERTY] = NGraphic::KEnum::TEXTURE_ID_RED;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		obj.get()->setScale(Vector3(0.5, 0.5, 0.5));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(i,0,0);
	}

}
