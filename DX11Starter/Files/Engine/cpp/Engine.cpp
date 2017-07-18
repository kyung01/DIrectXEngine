#include <Engine\Engine.h>

#include <iostream>		
#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;

void Engine::initExample()
{
	int ENTITY_NUMBER = 1;
	for (int j = 0; j < ENTITY_NUMBER; j++)for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = m_entityFactory.addEntity();
		m_renderSystem.addEntity(entity);
		Renderable& renderable = m_renderSystem.getComponent(j * ENTITY_NUMBER + (i));
		renderable.setPosition( Vector3(i,j,1));
	}
}

void KEngine::Engine::init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight)
{
	print("init");
	m_asset.init(device, context);
	m_renderSystem.init(windowWidth, windowHeight);
	initExample();
}
void Engine::update(float timeElapsed)
{
	//print("update");
	m_renderSystem.update(timeElapsed);
	m_inputSystem.update(timeElapsed);
	m_renderSystem.setCameraPosition(m_inputSystem.getPosition());
	m_renderSystem.setCameraRotation(m_inputSystem.getRotation());
}

void Engine::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
	//print("render");
	m_renderSystem.render(
		device, context, target, targetDepth, viewport, 
		m_asset.getRasterizer(KEnum::RASTR_CULLBACKFACE),
		m_asset.getVertShader(KEnum::SHADER_SIMPLE), m_asset.getFragShader(KEnum::SHADER_SIMPLE), 
		m_asset.m_meshes);
}
void Engine::OnMouseMove(WPARAM buttonState, int x, int y)
{
	m_inputSystem.OnMouseMove(buttonState, x, y);

	if (!(buttonState & 0x0001)) //if button is not pressed
	{
		return;
	}
}

void KEngine::Engine::OnResize(int windowWidth, int windowHeight)
{
	m_renderSystem.OnResize(windowWidth, windowHeight);
}
