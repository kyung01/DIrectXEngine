#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Asset.h>
#include <Engine\EntityFactory.h>
#include <Engine\Systems\LightRender.h>
#include <Engine\Systems\RenderSystem.h>
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
	class Engine {
	private:
		Asset		m_asset;
		EntityFactory m_entityFactory;
		KSystem::RenderSystem m_renderSystem;

	protected:
		void initExample();
	public:
		//pure data update
		void init(ID3D11Device * device, ID3D11DeviceContext * context);
		void update(float timeElapsed);
		void render(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport);

	};
}