#include <Engine\EntityFactory.h>
using namespace KEngine;

void KEngine::EntityFactory::addEntity(byte * entity, int size)
{
	int arraySize = m_entities.size();
	m_entities.resize(arraySize + size);
	for (int i = 0; i < size; i++) {
		m_entities[arraySize + i] = entity[i];
	}
	m_indexs.push_back({arraySize ,size });
	
}