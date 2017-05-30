#include <Engine\EntityFactory.h>
using namespace KEngine;
/*
void KEngine::EntityFactory::addEntity(byte * entity, int size)
{
	int arraySize = m_entities.size();
	m_entities.resize(arraySize + size);
	for (int i = 0; i < size; i++) {
		m_entities[arraySize + i] = entity[i];
	}
	m_indexs.push_back({arraySize ,size });
	
}
*/

Entity & KEngine::EntityFactory::addEntity()
{
	m_entities.resize(m_entities.size()+1);
	return m_entities[m_entities.size() - 1];
	// TODO: insert return statement here
}
