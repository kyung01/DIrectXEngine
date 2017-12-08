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
#include <Engine\Systems\ReflectionSystem.h>
#include <Engine\EventHandlers\PlayerInput.h>

#include <ClusteredFrustum\Frustum.h>
#include <ClusteredFrustum\BufferDataTranslator.h>

#include <Physics\PhysicsMain.h>

#include <DirectX\RenderTexture.h>
#include <DirectX\DepthTexture.h>

using namespace DirectX::SimpleMath;

namespace KEngine {
	class Engine  : EventHandler {
	private:
		//Temporary functions for easy modifying and testing
		Entity& addRenderableObjecet();
		Entity& addRenderableLightObjecet();
		Entity& addProbeReflection();
		Entity& addProbeIraadiance();
		void assignRandomTexture(Entity &entity);
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
		KSystem::ReflectionSystem	m_probeSystem;
		//Handlers
		KHandler::PlayerInput m_handlerKeyboardInput;
		//Test all kinds of light into this frustum
		KFrustum::Frustum				m_frustum;
		//Then translate all kinds of light and frustum data into this buffer
		KFrustum::BufferDataTranslator	m_dataTranslator;
		/// //Atals texture for rendering shadow maps for each light
		/// NDirectX::RenderTexture m_textureAtlasShadowMap;
		/// NDirectX::DepthTexture	m_textureAtalsShadowMapDepth;

	protected:
		void initExample();
	public:
		//Put on public for temporary debugging purposes
		D3D11_VIEWPORT debugViewport;
		NDirectX::RenderTexture debugCubemap;
		NDirectX::DepthTexture debugCubemapDepth;
	public:
		//Put on public temporarily for debug purpose
		//Atals texture for rendering shadow maps for each light
		NDirectX::RenderTexture m_textureAtlasShadowMap;
		NDirectX::DepthTexture	m_textureAtlasShadowMapDepth;

		//pure data update
		Engine();
		void init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight) override;
		void initTest(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight);
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