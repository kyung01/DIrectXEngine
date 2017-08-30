#include <Engine\Entity.h>
using namespace KEngine;

Entity::Entity() :
	m_transform3D(0),
	m_forwardLight(0),
	m_lightComponent(0),
	m_sphereicalHarmonicProbe(0)
{

}

/*
Entity::Entity(std::vector<byte> componenets, std::vector<ComponentIndex> indexs) :
	m_components(componenets),
	m_componentIndexs(indexs),
	m_isAlive(true),
	m_isDirty(true)
{
}
*/

/*
Entity Entity::addComponenet(Entity entity, Component componenet, ComponentId id, ComponentType compType, int compSize)
{
	auto componenets = entity.m_components;
	auto indexs = entity.m_componentIndexs;
	int componentVectorSize = componenets.size();
	indexs.push_back({id, compType ,componentVectorSize,compSize });
	componenets.resize(componentVectorSize + compSize);
	byte* componentByteAt = reinterpret_cast<byte*>(&componenet);
	for (int i = 0; i < compSize; i++) {
		componenets[componentVectorSize + i] = componentByteAt[i];
	}
	return{ entity.m_isAlive, entity.m_isDirty,componenets,indexs };
}

byte * KEngine::Entity::GetComponent(ComponentId id)
{
	int indexAt = -1;
	for (int i = 0; i < m_componentIndexs.size(); i++) {
		if (m_componentIndexs[i].id == id) {
			indexAt = m_componentIndexs[i].at;
			break;
		}
	}
	if (indexAt == -1) return 0;
	return &m_components[indexAt];
}

Entity KEngine::Entity::addComponenet(Component componenet, ComponentId id, ComponentType compType, int compSize)
{
	return addComponenet(*this, componenet,id, compType, compSize);
}
*/
