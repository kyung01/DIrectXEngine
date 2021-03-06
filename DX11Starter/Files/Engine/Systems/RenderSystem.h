#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Camera.h>
#include <Engine\Systems\System.h>
#include <Engine\Componenets\Renderable.h>
#include <Engine\SimpleShader.h>
#include <Engine\Asset.h>
#include <Engine\Mesh.h>
#include <Engine\EntityFactory.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class RenderSystem : public System<Renderable>{
		private:
		protected:
			Camera m_camera,m_cameraLight;
			DirectX::XMFLOAT4X4 matTemp; //temporary mat to store modified matrixs
			void setMatrix(ISimpleShader * shader, std::string name, DirectX::XMMATRIX matrix);
			void setRenderTarget(
				ID3D11DeviceContext* deviceContext,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport);
			void renderMesh(ID3D11DeviceContext * context, Mesh& mesh, UINT offsetBegin, UINT stride);

			//helper methods
			void addEntityHandle(Entity& entity, Renderable &componenet) override;
			void addEntityLinkRecreate(std::vector<Entity> & entityVectors, Renderable &componenet) override;


			UINT stride;// = sizeof(Vertex);
			UINT offset;// = 0;
			void hprRenderMesh(ID3D11DeviceContext * context,Asset &asset,KEnum meshID);
			void renderSkybox(
				//Render information needed to render
				ID3D11Device * device, ID3D11DeviceContext * context,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				Asset &asset,
				EntityFactory& entityFactory
			);
			void renderScene(
				//Render information needed to render
				ID3D11Device * device, ID3D11DeviceContext * context,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				Asset &asset,
				EntityFactory& entityFactory
			);

		public:
			void init(int renderTargetWidth, int renderTargetHeight);
			DirectX::SimpleMath::Matrix getCameraViewMatrix();
			
			Vector3 getCameraPosition();

			void setCameraProjectionFOV(float ratio);
			void setCameraPosition(Vector3 position);
			void setCameraRotation(Quaternion rotation);

			//void addEntity(Entity& entity) override;
			void update(std::vector<Entity> &entities, float time) override;
			void renderCubemap(
				//Render information needed to render
				ID3D11Device * device, ID3D11DeviceContext * context,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				Asset &asset,
				Renderable &renderable
			);
		
			void render(
				//Render information needed to render
				ID3D11Device * device, ID3D11DeviceContext * context,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				Asset &asset,
				ID3D11RasterizerState *cullBackFace,
				SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader,
				std::map<KEnum, Mesh> &meshes,
				EntityFactory& entityFactory
			);
			void renderPointLightShadowMap(
				ID3D11Device * device, ID3D11DeviceContext * context,
				Vector3 position,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				ID3D11RasterizerState *cullBackFace,
				SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader,
				std::map<KEnum, Mesh> &meshes,
				EntityFactory& entityFactory
			);
			void renderSpotLightShadowMap(
				ID3D11Device * device, ID3D11DeviceContext * context,
				Vector3 position, Quaternion rotation, float fov,
				ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
				ID3D11RasterizerState *cullBackFace,
				SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader,
				std::map<KEnum, Mesh> &meshes,
				EntityFactory& entityFactory
			);

			void OnResize(int targetFrameWidth, int targetFrameHeight);
		};
	}

}