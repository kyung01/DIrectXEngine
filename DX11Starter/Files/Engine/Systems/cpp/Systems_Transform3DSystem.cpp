#include <Engine\Systems\Transform3DSystem.h>

void KEngine::KSystem::Transform3DSystem::addEntityHandle(Entity & entity, Transform3D & componenet)
{
	entity.m_transform3D = &componenet;
}

void KEngine::KSystem::Transform3DSystem::addEntityLinkRecreate(Entity & entity, Transform3D & componenet)
{
	entity.m_transform3D = &componenet;
}

void KEngine::KSystem::Transform3DSystem::update(std::vector<Entity>& entities, float timeElapsed)
{
	for (int i = 0; i < m_components.size(); i++) {
		m_components[i].isDirty = false;
	}
}
