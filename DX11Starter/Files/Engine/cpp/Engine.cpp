#include <Engine\Engine.h>

#include <iostream>		
#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;

void Engine::initExample()
{
	int ENTITY_NUMBER = 10;
	for (int j = 0; j < ENTITY_NUMBER; j++)for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = m_entityFactory.addEntity();
		m_renderSystem.addEntity(entity);
		Renderable& renderable = m_renderSystem.getComponent(j * ENTITY_NUMBER + (i));
		renderable.position = Vector3(i,j,1);
	}
}

void KEngine::Engine::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	print("init");
	m_asset.init(device, context);
	initExample();
}
void Engine::update(float timeElapsed)
{
	print("update");
}

void Engine::render(ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
	print("render");
}
