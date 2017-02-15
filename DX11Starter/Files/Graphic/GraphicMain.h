#pragma once
#include <map>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "SimpleShader.h"
#include "RenderTexture.h"
#include "DepthTexture.h"
#include "Graphic\Scene\Light.h"
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Graphic\Asset\Asset.h>
#include <Graphic\Asset\Mesh.h>// TODO delete this
#include <Graphic\Enum.h>
#include <Graphic\RenderStateStack.h>
#include <Graphic\Buffer\KDynamicBuffer.h>
#include <Graphic\Buffer\LightParameter.h>
#include <Game\Context.h>

#include <Graphic\Scene\Scene.h>


namespace NGraphic {
	
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct LightInfo{
		std::shared_ptr<RenderTexture>	position;
		std::shared_ptr<DepthTexture>	depth;
	};

	class GraphicMain {
	private:
		static float RATIO_LIGHT_INNER;
		DirectX::XMMATRIX orthoView, orthoMVP;
		RenderStateStack m_renderStackStack;
		RenderTexture	m_renderTextureDummy;
		DepthTexture	m_depthTextureDummy;
		int m_rsm_flux_eye_perspective_width, m_rsm_flux_eye_perspective_height;
	
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::LightParameter>> m_lightBuffer;


		DirectX::XMMATRIX getOrthogonalMatrixProj();
		DirectX::XMMATRIX getOrthogonalMatrixView();
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);

		LightInfo getLightInfo(ID3D11Device *device);
		bool initTextures		(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight);
		
		
	protected:
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(NScene::Camera cam);// = 0;
		void beginRendering(ID3D11DeviceContext *context);// = 0;
		void endRendering(ID3D11DeviceContext *context);// = 0;
		void getScreenWidth(int &w, int &h);// = 0;

		void drawLine(
			ID3D11Device *device, ID3D11DeviceContext *context, Matrix worldMatrix, Mesh model,
			Vector3 from, Vector3 end);
		
	public:
		int m_width, m_height;
		Mesh * mesh00;


		Frustum m_frustum;
		std::map<int, LightInfo> m_lightInfos;
		std::map<KEnum, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<KEnum, std::shared_ptr<DepthTexture>>	m_depthTextures;

		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int textureWidth, int textureHeight, int textureIndirectLightWidth, int textureIndirectLightHeight);
		void update(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, NScene::Scene & scene);
		
		
		void renderWorldNormalDiffuse(ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset, NScene::Scene& scene, DirectX::SimpleMath::Matrix worldMatrix);//rendering the normal scene
		void renderUI(ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset, NScene::Scene& scene, DirectX::SimpleMath::Matrix worldMatrix);//rendering the normal scene

		void renderWorld(ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset,
			NScene::Scene& scene,
			DirectX::SimpleMath::Matrix& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
			RenderTexture &renderTexture, DepthTexture& depthTexture);//rendering the normal scene
		void renderFrustum(
			ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset,
			Vector3 eyePosition,
			DirectX::XMMATRIX& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
			RenderTexture &renderTexture, DepthTexture &depthTexture,
			RenderTexture &renderTexture2 , DepthTexture &depthTexture2 );//rendering the normal scene

		void render(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
			Asset& asset, NGame::Context &game);
		void renderDebug(
			ID3D11Device * device, ID3D11DeviceContext * context,
			RenderTexture& renderTexture, DepthTexture& depthTexture,
			
			Asset& asset, NGame::Context &game, NScene::Scene& scene
			);

		void renderSkyboxReflection(
			ID3D11Device * device, ID3D11DeviceContext *context,
			RenderTexture& renderTexture, DepthTexture& depthTexture,
			Asset& asset,
			Vector3 eyePos, 
			ID3D11ShaderResourceView *skybox, ID3D11ShaderResourceView * worldPos, ID3D11ShaderResourceView * normalTexture);
		void renderDirectLight(
			ID3D11Device * device, ID3D11DeviceContext *context,
			RenderTexture& renderTexture, DepthTexture& depthTexture,
			Asset& asset,
			Vector3 eyePos,
			Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter,
			std::shared_ptr<RenderTexture> lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV);
		void renderLightShaft(
			ID3D11Device * device, ID3D11DeviceContext *context,
			RenderTexture& renderTexture, DepthTexture& depthTexture,
			Asset& asset,
			Vector3 eyePos, Vector3 eyeLook,
			Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter,
			std::shared_ptr<RenderTexture> frustumFront,
			std::shared_ptr<RenderTexture> frustumBack,

			std::shared_ptr<RenderTexture> lightShadow, 
			DirectX::XMMATRIX lightMVP, float lightFOV);
	};
}