#include <Engine\Systems\ReflectionSystem.h>

using namespace KEngine::KSystem;

//Private + Protected

void KEngine::KSystem::ReflectionSystem::addEntityHandle(Entity & entity, ReflectionComponent & componenet)
{
	//Nothing
}
void KEngine::KSystem::ReflectionSystem::addEntityLinkRecreate(std::vector<Entity>& entityVectors, ReflectionComponent & componenet)
{
	entityVectors[componenet.entityIndex].m_probeComponent = &componenet;
}

//Public

ReflectionSystem::ReflectionSystem()
{
}

void KEngine::KSystem::ReflectionSystem::init()
{
}

void KEngine::KSystem::ReflectionSystem::update(std::vector<Entity>& entities, float time)
{
}
