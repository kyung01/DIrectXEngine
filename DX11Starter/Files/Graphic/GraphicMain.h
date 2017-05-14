#pragma once
#include <map>
#include "Model.h"
#include "Camera.h"

#include "SimpleShader.h"
#include "DepthTexture.h"
#include <Graphic\Scene\SpotLight.h>
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>
#include <WICTextureLoader.h>
//#include <Graphic\RenderTexture.h>
#include <Graphic\StagingTexture.h>
#include <Graphic\Scene\Scene.h>
#include <Graphic\LightInfo.h>
#include <Graphic\Asset\Asset.h>
#include <Graphic\Asset\Mesh.h>// TODO delete this
#include <Graphic\Enum.h>
#include <Graphic\RenderStateStack.h>
#include <Graphic\TextureAtlasSlicer.h>
#include <Graphic\BufferDataTranslator.h>
#include <Graphic\Buffer\KDynamicBuffer.h>
#include <Graphic\Buffer\LightParameter.h>
#include <Graphic\StagingTexture.h>
#include <Game\Context.h>
#include <Graphic\Scene\Probe.h>

namespace NGraphic {

	class GraphicMain {
	private:

		bool m_isProbeReady = false;

		static float RATIO_LIGHT_INNER;
		int m_width, m_height;
		DirectX::XMMATRIX	orthoView, orthoMVP;
		RenderStateStack	m_renderStackStack;
		StagingTexture		m_probePrebake;
		StagingTexture		m_probeBaked;
		RenderTexture		m_probeCubemapTarget;
		DepthTexture		m_probeCubemapDepth;

		RenderTexture		m_renderTextureDummy;
		RenderTexture		m_probeCubeArray;
		DepthTexture		m_depthTextureDummy;
		int					m_rsm_flux_eye_perspective_width, 
							m_rsm_flux_eye_perspective_height;

		
		std::shared_ptr<NBuffer::KDynamicBuffer<NBuffer::LightParameter>> m_lightBuffer;
		std::shared_ptr<BufferDataTranslator> m_bufferDataTranslator;





		DirectX::XMMATRIX getOrthogonalMatrixProj();
		DirectX::XMMATRIX getOrthogonalMatrixView();
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);

		void renderLightAtlas(
			ID3D11Device * device, ID3D11DeviceContext * context, Asset& asset, NScene::Scene &scene);
		
		Vector3 getSpearNormal(int face, float pixelSize, float u, float v);
	protected:
		LightInfo getLightInfo(ID3D11Device *device);
		bool initTextures(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight);



		//glm::mat4 matProjection, matView, matModel;
		void processCamera(NScene::Camera cam);// = 0;
		void beginRendering(ID3D11DeviceContext *context);// = 0;
		void endRendering(ID3D11DeviceContext *context);// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
		void updateBufferLightPrameter(ID3D11DeviceContext *context, ID3D11Buffer* buffer, std::list<std::shared_ptr<NScene::SpotLight>> &lights);
		void updateLightAtlas(std::list<std::shared_ptr<NScene::SpotLight>> &lights);

		
		void renderProbe(ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene,
			NScene::Probe &probe,RenderTexture & probeTarget, DepthTexture & probeDepth);

		void renderDeferred();
		void renderClusteredForwardRendering(

			ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene,
			ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport,
			RenderTexture & textureAtlas, DepthTexture & depthAtlas,
			Matrix & matWorld, Matrix& matView, Matrix & matProj,
			int frustumSizeX, int frustumSizeY, int frustumSizeZ,
			float frustumFov, float frustumNear, float frustumFar, float frustumWidthOverHeight
		);
	public:
		// Eventually move to private or protected access level but for now, put it in a public for debug purpose
		NFrustum::Frustum				
			m_frustumLight,
			m_frustumProbe
			;
		std::shared_ptr<TextureAtlasSlicer> m_atlasSlicer;

		std::map<int, LightInfo> m_lightInfos;
		std::map<KEnum, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<KEnum, std::shared_ptr<DepthTexture>>	m_depthTextures;

		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int textureWidth, int textureHeight, int textureIndirectLightWidth, int textureIndirectLightHeight);
		void update(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene);
		void updateProbes(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene);
		
		
		void updateUnInitializedObjects(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene);
		void updateFrustum(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, 
			Asset & asset,
			NGraphic::NFrustum::Frustum &frustum,
			DirectX::SimpleMath::Matrix camViewMatrix, 
			std::list < std::shared_ptr< NScene::ILight> > lights);



		void renderClusteredForward(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport,
			Asset& asset,
			NGraphic::NFrustum::Frustum &frustum,
			
	SimpleMath::Matrix worldMatrix,
	SimpleMath::Matrix viewMatirx,
	SimpleMath::Matrix projMatrix,
			NScene::Scene& scene);
		void renderDeffered(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
			Asset& asset, NGame::Context &game);
		
	
	};
}
/*
Implement the following function later 
void renderSkyboxReflection(
ID3D11Device * device, ID3D11DeviceContext *context,
RenderTexture& renderTexture, DepthTexture& depthTexture,
Asset& asset,
Vector3 eyePos,
ID3D11ShaderResourceView *skybox, ID3D11ShaderResourceView * worldPos, ID3D11ShaderResourceView * normalTexture);


void NGraphic::GraphicMain::renderSkyboxReflection(
ID3D11Device * device, ID3D11DeviceContext * context, RenderTexture & renderTexture, DepthTexture & depthTexture,
Asset & asset,
Vector3 eyePos,
ID3D11ShaderResourceView *skybox, ID3D11ShaderResourceView * worldPos, ID3D11ShaderResourceView * normalTexture)
{
beginRendering(context);

context->OMSetBlendState(asset.BLEND_STATE_TRANSPARENT, 0, 0xffffffff);

DirectX::XMFLOAT4X4 matrixStore;
NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_PLANE];
SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_SKYBOX_REFLECTION];
SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_SKYBOX_REFLECTION];

renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());

DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(orthoMVP)); // Transpose for HLSL!
shaderVert.SetMatrix4x4("matMVP", matrixStore);

shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
shaderFrag.SetShaderResourceView("textureSky", skybox);
shaderFrag.SetShaderResourceView("textureWorld", worldPos);
shaderFrag.SetShaderResourceView("textureNormal", normalTexture);
shaderFrag.SetFloat3("eyePos", eyePos);

shaderVert.SetShader();
shaderFrag.SetShader();
shaderVert.CopyAllBufferData();
shaderFrag.CopyAllBufferData();

UINT stride = sizeof(Vertex);
UINT offset = 0;
context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
context->DrawIndexed(
mesh.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
0,     // Offset to the first index we want to use
0);    // Offset to add to each index when looking up vertices

//clean up bindings
shaderFrag.SetShaderResourceView("textureFrustumFront", 0);
shaderFrag.SetShaderResourceView("textureFrustumBack", 0);
shaderFrag.SetShaderResourceView("textureShadow", 0);
shaderFrag.SetShaderResourceView("textureWorld", 0);

endRendering(context);
}

*/