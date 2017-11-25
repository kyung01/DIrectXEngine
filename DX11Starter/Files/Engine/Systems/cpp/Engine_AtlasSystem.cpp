#include <Engine\Systems\AtlasSystem.h>
#include <Engine\Entity.h>
#include <Engine\Componenets\LightEntity.h>
#include <iostream>

using namespace KEngine;
using namespace KEngine::KSystem;

AtlasSystem::AtlasSystem()
{
}

void AtlasSystem::init(int width, int height, int spotLightWidth, int spotLightHeight, int pointLightWidth, int pointLightHeight)
{
	m_width = width;
	m_height = height;
	m_spotLight_Width = spotLightWidth;
	m_spotLight_Height = spotLightHeight;
	m_pointLight_Width = pointLightWidth;
	m_pointLight_Height = pointLightHeight;
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
	float sliceWidth, sliceHeight;
	if (entity.m_lightComponent->lightType == LIGHT_TYPE::SPOT_LIGHT) {
		sliceWidth = m_spotLight_Width;
		sliceHeight = m_spotLight_Width;
	}
	else {
		sliceWidth = m_pointLight_Width;
		sliceHeight = m_pointLight_Height;
	}
	do
	{
		offsetX = xBegin;
		offsetY = yBegin;
		
		//check if this is renderable size
		if (offsetX + sliceWidth + 1 > m_width) {
			xBegin = 1;
			yBegin += sliceHeight +2;
			continue;
		}
		if (offsetY + sliceHeight + 1 > m_height) {
			isAtalsTextureNotBigEnough = true;
		}

		if (isAtalsTextureNotBigEnough) {
			std::cout << "Engine AtlasSystem Error. Given atlas texture was not big enough to slice.";
			return;
		}
		componenet.x = xBegin;
		componenet.y = yBegin;
		componenet.width = sliceWidth;
		componenet.height = sliceHeight;
		xBegin += (2 + sliceWidth);
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
