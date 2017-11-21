#include <Engine\Systems\AtlasSystem.h>
#include <iostream>

using namespace KEngine;
using namespace KEngine::KSystem;

AtlasSystem::AtlasSystem()
{
}

void AtlasSystem::init(int width, int height, int sliceWidth, int sliceHeight)
{
	m_width = width;
	m_height = height;
	m_sliceWidth = sliceWidth;
	m_sliceHeight = sliceHeight;
	//leave an empty pixel
	xBegin = 1;
	yBegin = 1;
}

void AtlasSystem::addEntityHandle(Entity & entity, AtlasComponent & componenet)
{
	bool isValidCoordinateFound = false;
	bool isAtalsTextureNotBigEnough = false;
	int offsetX, offsetY; 
	int width, height;
	do
	{
		offsetX = xBegin;
		offsetY = yBegin;
		
		//check if this is renderable size
		if (offsetX + m_sliceWidth + 1 > m_width) {
			xBegin = 1;
			yBegin += m_sliceHeight+2; 
			continue;
		}
		if (offsetY + m_sliceHeight + 1 > m_height) {
			isAtalsTextureNotBigEnough = true;
		}

		if (isAtalsTextureNotBigEnough) {
			std::cout << "Engine AtlasSystem Error. Given atlas texture was not big enough to slice.";
			return;
		}
		componenet.x = xBegin;
		componenet.y = yBegin;
		componenet.width = m_sliceWidth;
		componenet.height = m_sliceHeight;
		xBegin += (2 + m_sliceWidth);
		isValidCoordinateFound = true;

	} while (!isValidCoordinateFound);

}

void AtlasSystem::addEntityLinkRecreate(std::vector<Entity>& entityVectors, AtlasComponent & componenet)
{
	entityVectors[componenet.entityIndex].m_atlasComponent = &componenet;
}

void AtlasSystem::update(std::vector<Entity>& entities, float time)
{
}
