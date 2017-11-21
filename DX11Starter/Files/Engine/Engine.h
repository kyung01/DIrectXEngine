#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete

#include <Engine\EventHandler.h>
#include <Engine\Asset.h>
#include <Engine\EntityFactory.h>
#include <Engine\Systems\AtlasSystem.h>
#include <Engine\Systems\RenderSystem.h>
#include <Engine\Systems\LightSystem.h>
#include <Engine\Systems\Transform3DSystem.h>
#include <Engine\EventHandlers\PlayerInput.h>

#include <ClusteredFrustum\Frustum.h>
#include <ClusteredFrustum\BufferDataTranslator.h>

#include <Physics\PhysicsMain.h>

using namespace DirectX::SimpleMath;

namespace KEngine {
	class Engine  : EventHandler {
	private:
		Physics::PhysicsMain m_physicsMain;
		Asset		m_asset;
		EntityFactory m_entityFactory;
		std::list<EventHandler*> m_eventHandlers;
		//Systems
		KSystem::RenderSystem	m_renderSystem;
		KSystem::AtlasSystem	m_atlasSystem;
		KSystem::LightSystem	m_lightSystem;
		KSystem::Transform3DSystem m_transform3DSystem;
		//Handlers
		KHandler::PlayerInput m_handlerKeyboardInput;

		KFrustum::Frustum				m_frustum;
		KFrustum::BufferDataTranslator	m_dataTranslator;
	protected:
		void initExample();
	public:
		//pure data update
		Engine();
		void init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight) override;
		void update(float timeElapsed) override;
		void renderUpdate(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport);
		void render(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport) override;
		void OnMouseMove(WPARAM buttonState, int x, int y) override;
		void OnResize(int windowWidth, int windowHeight) override;
	};
}