#include <Engine\Engine.h>

#include <iostream>		
#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;
KEngine::Engine::Engine()
{
	m_eventHandlers.push_back(&m_handlerKeyboardInput);
}
void Engine::initExample()
{
	int ENTITY_NUMBER = 10;
	for (int j = 0; j < ENTITY_NUMBER; j++)for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = m_entityFactory.addEntity();

		//m_renderSystemFlawed.addEntity(entity);
		//Renderable& renderable = m_renderSystemFlawed.getComponent(j * ENTITY_NUMBER + (i));
		//renderable.setPosition( Vector3(i,j,1));

		m_renderSystem.addEntity(entity);
		auto& compRenderable = m_renderSystem.getComponent(j * ENTITY_NUMBER + (i));
		compRenderable.setPosition(Vector3(i, j, 1));

	}
}



void KEngine::Engine::init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight)
{
	print("init");
	m_asset.init(device, context);
	m_renderSystemFlawed.init(windowWidth, windowHeight);
	initExample();
}
void Engine::update(float timeElapsed)
{

	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->update(timeElapsed);
	}
	m_renderSystem.setCameraPosition(m_handlerKeyboardInput.getPosition());
	m_renderSystem.setCameraRotation(m_handlerKeyboardInput.getRotation());


	//print("update");
	//m_renderSystemFlawed.update(timeElapsed);
	//m_inputSystem.update(timeElapsed);
	//m_renderSystemFlawed.setCameraPosition(m_inputSystem.getPosition());
	//m_renderSystemFlawed.setCameraRotation(m_inputSystem.getRotation());
}

void Engine::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
	//print("render");
	//m_renderSystemFlawed.render(
	//	device, context, target, targetDepth, viewport,
	//	m_asset.getRasterizer(KEnum::RASTR_CULLBACKFACE),
	//	m_asset.getVertShader(KEnum::SHADER_SIMPLE), m_asset.getFragShader(KEnum::SHADER_SIMPLE),
	//	m_asset.m_meshes);
	m_renderSystem.render(
		device, context, target, targetDepth, viewport,
		m_asset.getRasterizer(KEnum::RASTR_CULLBACKFACE),
		m_asset.getVertShader(KEnum::SHADER_SIMPLE), m_asset.getFragShader(KEnum::SHADER_SIMPLE),
		m_asset.m_meshes);
}
void Engine::OnMouseMove(WPARAM buttonState, int x, int y)
{
	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->OnMouseMove(buttonState, x, y);
	}

	if (!(buttonState & 0x0001)) //if button is not pressed
	{
		return;
	}
}

void KEngine::Engine::OnResize(int windowWidth, int windowHeight)
{
	m_renderSystemFlawed.OnResize(windowWidth, windowHeight);
}
