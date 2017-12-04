#include <Engine\Systems\ProbeSystem.h>

using namespace KEngine::KSystem;

//Private + Protected

void KEngine::KSystem::ProbeSystem::addEntityHandle(Entity & entity, ProbeComponent & componenet)
{
	//Nothing
}
void KEngine::KSystem::ProbeSystem::addEntityLinkRecreate(std::vector<Entity>& entityVectors, ProbeComponent & componenet)
{
	entityVectors[componenet.entityIndex].m_probeComponent = &componenet;
}

//Public

ProbeSystem::ProbeSystem()
{
}

void KEngine::KSystem::ProbeSystem::init()
{
}

void KEngine::KSystem::ProbeSystem::update(std::vector<Entity>& entities, float time)
{
}
