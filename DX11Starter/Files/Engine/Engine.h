#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete

#include <Engine\EventHandler.h>
#include <Engine\Asset.h>
#include <Engine\EntityFactory.h>
#include <Engine\Systems\LightRender.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\RenderSystemFlawed.h>
#include <Engine\Systems\InputSystem.h>
#include <Engine\EventHandlers\PlayerInput.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
//namespace KEngine {
//	class EntityFactory;
//	class Entity;
//	namespace KSystem {
//		class RenderSystem;
//	}
//}
namespace KEngine {
	class Engine  : EventHandler {
	private:
		Asset		m_asset;
		EntityFactory m_entityFactory;
		std::list<EventHandler*> m_eventHandlers;
		//Systems
		KSystem::RenderSystem m_renderSystem;
		KSystem::RenderSystemFlawed m_renderSystemFlawed;
		KSystem::InputSystem m_inputSystem;
		//Handlers
		KHandler::PlayerInput m_handlerKeyboardInput;
	protected:
		void initExample();
	public:
		//pure data update
		Engine();
		void init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight) override;
		void update(float timeElapsed) override;
		void render(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport) override;
		void OnMouseMove(WPARAM buttonState, int x, int y) override;
		void OnResize(int windowWidth, int windowHeight) override;
	};
}