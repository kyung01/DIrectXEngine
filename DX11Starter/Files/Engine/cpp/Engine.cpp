#include <Engine\Engine.h>
#include <iostream>		
#include <DirectX\DirectXUtility.h>

#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;

void Engine::initExample()
{
	int ENTITY_NUMBER = 50;
	int RANDOM_LIGHT_NUMBER = 10;
	for(int i = 0; i < ENTITY_NUMBER; i++)
	{
		//m_renderSystemFlawed.addEntity(entity);
		//Renderable& renderable = m_renderSystemFlawed.getComponent(j * ENTITY_NUMBER + (i));
		//renderable.setPosition( Vector3(i,j,1));
		Entity& entity = m_entityFactory.addEntity();
		m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity);
		m_renderSystem.addEntity(m_entityFactory.m_entities, entity);
		//m_renderSystem.getLastComponent().setPosition(Vector3(i, j, 1));
	}

	int x(0), y(0), z(3);
	for (int i = m_transform3DSystem.getComponentVectorSize() - 1; i >= 0; i--) {
		m_transform3DSystem.getComponent(i).setPosition(x++, y, z);
		if (x > 10) {
			x = 0;
			y++;
		}
	}
	for (int i = 0; i < RANDOM_LIGHT_NUMBER; i++) {
		int selectedEntityIndex;
		bool isNewEntityAvailable = false;
		int maxTolerableFailure = 100;
		do {
			selectedEntityIndex = DirectX::DirectXUtility::GET_RANDOM() % m_entityFactory.m_entities.size();
			isNewEntityAvailable = (m_entityFactory.getEntity(selectedEntityIndex).m_lightComponent == 0);
		} while (!isNewEntityAvailable && maxTolerableFailure-- > 0 );
		if (!isNewEntityAvailable) continue;
		m_lightSystem.addEntity(m_entityFactory.m_entities, m_entityFactory.getEntity(selectedEntityIndex));
	}
}

KEngine::Engine::Engine() :
	m_dataTranslator(10, 10, 10, 10, 10)
{
	m_eventHandlers.push_back(&m_handlerKeyboardInput);
}

void KEngine::Engine::init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight)
{
	print("init");
	m_asset.init(device, context);
	m_lightSystem.init(windowWidth/windowHeight, 0.01f, 100.0f, 10, 10, 10);
	m_frustum.init(windowWidth / (float)windowHeight, 0.1f, 100.0f, 10, 10, 10);
	initExample();
}
void Engine::update(float timeElapsed)
{
	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->update(timeElapsed);
	}
	m_renderSystem.setCameraPosition(m_handlerKeyboardInput.getPosition());
	m_renderSystem.setCameraRotation(m_handlerKeyboardInput.getRotation());
	m_renderSystem		.update(m_entityFactory.m_entities, timeElapsed);
	m_transform3DSystem	.update(m_entityFactory.m_entities, timeElapsed);
	m_lightSystem		.update(m_entityFactory.m_entities, timeElapsed);


	auto camViewMatrix = m_renderSystem.getCameraViewMatrix();


	for (int i = 0; i < m_lightSystem.getLightCount(); i++) {
		Vector3 lightPos, lightDirLook;
		float lightIntensity, lightFOV;
		//From Camera's perspective
		Vector3 pos, posDirLook, dir;
		LIGHT_TYPE lightType = m_lightSystem.getLightType(i);

		switch (lightType) {
		case LIGHT_TYPE::POINT_LIGHT:
			lightPos = m_lightSystem.getPointLight(i).position;
			lightIntensity = m_lightSystem.getPointLight(i).intensity;
			break;
		case LIGHT_TYPE::SPOT_LIGHT:
			lightPos = m_lightSystem.getSpotLight(i).position;
			lightDirLook = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_lightSystem.getSpotLight(i).rotation);

			lightIntensity = m_lightSystem.getSpotLight(i).intensity;
			lightFOV = m_lightSystem.getSpotLight(i).fov;
			break;
		}

		pos = DirectX::XMVector3Transform(lightPos, camViewMatrix);
		posDirLook = XMVector3Transform(lightPos + lightDirLook, camViewMatrix);
		dir = posDirLook - pos;

		
		if (m_lightSystem.getLightType(i) == LIGHT_TYPE::POINT_LIGHT) {
			m_frustum.testPointlight(i, pos,lightIntensity);
		}
		else if (m_lightSystem.getLightType(i) == LIGHT_TYPE::SPOT_LIGHT) {
			m_frustum.testSpotlight(i, pos, dir, lightIntensity, lightFOV);
		}


	}
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
	m_renderSystem.OnResize(windowWidth, windowHeight);
}
