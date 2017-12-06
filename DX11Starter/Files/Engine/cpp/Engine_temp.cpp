#include <Engine\Engine.h>

using namespace KEngine;

Entity& Engine::addRenderableObjecet() {
	Entity& entity = m_entityFactory.addEntity();
	int entityIndex = m_entityFactory.m_entities.size() - 1;
	m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
	m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
	m_renderSystem.getLastComponent().meshId = MESH_SPHERE;
	return entity;
}
Entity& Engine::addRenderableLightObjecet() {
	Entity& entity = m_entityFactory.addEntity();
	int entityIndex = m_entityFactory.m_entities.size() - 1;
	m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
	m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
	m_renderSystem.getLastComponent().meshId = MESH_SPHERE;
	m_lightSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
	m_atlasSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);


	return entity;
}
void Engine::assignRandomTexture(Entity & entity)
{
	switch (rand() % 9) {
	default:
	case 0:
		entity.m_renderable->albedoMap = TXTURE_GOLD_SCUFFED_ALBD;
		entity.m_renderable->normalMap = TXTURE_GOLD_SCUFFED_NORMAL;
		entity.m_renderable->roughMap = TXTURE_GOLD_SCUFFED_ROUGH;
		entity.m_renderable->metalMap = TXTURE_GOLD_SCUFFED_METALNESS;
		break;
	case 1:
		entity.m_renderable->albedoMap = TXTURE_METAL_ALBD;
		entity.m_renderable->roughMap = TXTURE_METAL_ROUGH;
		entity.m_renderable->metalMap = TXTURE_METAL_METALNESS;
		break;
	case 2:
		entity.m_renderable->albedoMap = TXTURE_METAL_RUSTED_ALBD;
		entity.m_renderable->normalMap = TXTURE_METAL_RUSTED_NORMAL;
		entity.m_renderable->roughMap = TXTURE_METAL_RUSTED_ROUGH;
		entity.m_renderable->metalMap = TXTURE_METAL_RUSTED_METALNESS;
		entity.m_renderable->aoMap = TXTURE_METAL_RUSTED_AO;
		break;
	case 3:
		entity.m_renderable->albedoMap = TXTURE_METAL_GREASY_ALBD;
		entity.m_renderable->normalMap = TXTURE_METAL_GREASY_NORMAL;
		entity.m_renderable->roughMap = TXTURE_METAL_GREASY_ROUGH;
		entity.m_renderable->metalMap = TXTURE_METAL_GREASY_METALNESS;
		break;
	case 4:
		entity.m_renderable->albedoMap = TXTURE_COPPER_ROCK_ALBD;
		entity.m_renderable->normalMap = TXTURE_COPPER_ROCK_NORMAL;
		entity.m_renderable->roughMap = TXTURE_COPPER_ROCK_ROUGH;
		entity.m_renderable->metalMap = TXTURE_COPPER_ROCK_METALNESS;
		entity.m_renderable->aoMap = TXTURE_COPPER_ROCK_AO;
		break;
	case 5:
		entity.m_renderable->albedoMap = TXTURE_ROCK_ALBD;
		entity.m_renderable->normalMap = TXTURE_ROCK_NORMAL;
		entity.m_renderable->roughMap = TXTURE_ROCK_ROUGH;
		entity.m_renderable->metalMap = TXTURE_ROCK_METALNESS;
		entity.m_renderable->aoMap = TXTURE_ROCK_AO;
		break;
	case 6:
		entity.m_renderable->albedoMap = TXTURE_MARBEL_ALBD;
		entity.m_renderable->normalMap = TXTURE_MARBEL_NORMAL;
		entity.m_renderable->roughMap = TXTURE_MARBEL_ROUGH;
		entity.m_renderable->metalMap = TXTURE_MARBEL_METALNESS;
		break;
	case 7:
		entity.m_renderable->albedoMap = TXTURE_MAHOGFLOOR_ALBD;
		entity.m_renderable->normalMap = TXTURE_MAHOGFLOOR_NORMAL;
		entity.m_renderable->roughMap = TXTURE_MAHOGFLOOR_ROUGH;
		entity.m_renderable->aoMap = TXTURE_MAHOGFLOOR_AO;
		break;
	case 8:
		entity.m_renderable->albedoMap = TXTURE_HARSHBRICKS_ALBD;
		entity.m_renderable->normalMap = TXTURE_HARSHBRICKS_NORMAL;
		entity.m_renderable->roughMap = TXTURE_HARSHBRICKS_ROUGH;
		entity.m_renderable->metalMap = TXTURE_HARSHBRICKS_METALNESS;
		entity.m_renderable->aoMap = TXTURE_HARSHBRICKS_AO;
		break;
	}
}
void Engine::initExample()
{

	int ENTITY_NUMBER = 64;
	int RANDOM_MODEL_NUMBER = 5;
	int RANDOM_LIGHT_NUMBER = 5;
	int x(0), y(0), z(3);

	for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = addRenderableObjecet();
		assignRandomTexture(entity);
		entity.m_transform3D->setPosition(-4 + x++, -4 + y, z);
		entity.m_transform3D->setRotation(Quaternion::CreateFromAxisAngle(Vector3(rand(), rand(), rand()), rand()));
		if (x > 8) {
			x = 0;
			y++;
		}
	}
	for (int i = 0; i < RANDOM_LIGHT_NUMBER; i++) {
		Entity& entity = addRenderableLightObjecet();
		entity.m_lightComponent->color = Vector3(1, 1, 1);
		entity.m_renderable->meshId = (m_lightSystem.getLastComponent().lightType == LIGHT_TYPE::POINT_LIGHT)?
			MESH_SPHERE : MESH_SPOTLIGHT;
		entity.m_transform3D->setPosition(-4+rand()%9,-4+rand()%9,-4);
	}
	{

		Vector3 positions[4] = { Vector3(0, 0, -5) ,Vector3(0, 0, +5) ,Vector3(-5, 0, 0),Vector3(+5, 0, 0) };
		float rotations[4] = { 0,3.14f,3.14f / 2.0f,-3.14f / 2.0f };
		for (int i = 0; i < 4; i++)
		{
			Entity& entity = m_entityFactory.addEntity();
			int entityIndex = m_entityFactory.m_entities.size() - 1;
			m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.getLastComponent().meshId = MESH_PLANE;
			m_transform3DSystem.getLastComponent().setPosition(positions[i]);
			m_transform3DSystem.getLastComponent().setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), rotations[i]));
			m_transform3DSystem.getLastComponent().setScale(Vector3(10, 10, 10));

		}
	}
	{

		Vector3 positions[2] = { Vector3(0, -5, 0) , Vector3(0, 5, 0) };
		float rotations[2] = { -3.14f / 2, 3.14f / 2 };
		for (int i = 0; i < 1; i++)
		{
			Entity& entity = m_entityFactory.addEntity();
			int entityIndex = m_entityFactory.m_entities.size() - 1;
			m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.getLastComponent().meshId = MESH_PLANE;
			m_transform3DSystem.getLastComponent().setPosition(positions[i]);
			m_transform3DSystem.getLastComponent().setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), rotations[i]));
			m_transform3DSystem.getLastComponent().setScale(Vector3(10, 10, 10));
		}
	}
}