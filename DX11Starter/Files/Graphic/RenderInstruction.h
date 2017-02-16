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
	class RenderInstruction {

		static DirectX::XMFLOAT4X4 matWorld, matView, matProj;
		static void RENDER_LIGHTS(ID3D11DeviceContext * context, SimpleVertexShader& shaderVertSimpleColor, SimpleFragmentShader& DshaderFrag, Mesh& mesh, XMMATRIX& worldMatrix);
	public:
		
		static void RENDER_FRUSTUM(
			ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset,
			Vector3 eyePosition,
			DirectX::XMMATRIX& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
			RenderTexture &renderTexture, DepthTexture &depthTexture,
			RenderTexture &renderTexture2, DepthTexture &depthTexture2);//rendering the normal scene
		static void RENDER_LIGHTSHAFT_SPOTLIGHT(
			ID3D11Device * device, ID3D11DeviceContext * context, 
			Asset & asset,
			RenderTexture & renderTexture, DepthTexture & depthTexture,
			DirectX::XMMATRIX orthoMVP,
			Vector3 eyePos, Vector3 eyeLook,
			Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter,float lightDensity,
			RenderTexture & world,
			RenderTexture &frustumFront, RenderTexture & frustumBack,
			RenderTexture & lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV);
		static void RENDER_DEBUG(
			ID3D11Device * device, ID3D11DeviceContext * context,
			Asset& asset,
			RenderTexture& renderTexture, DepthTexture& depthTexture,
			NGame::Context& game, NScene::Scene& scene, Frustum& m_frustum,
			DepthTexture& textureEyeDepth);
		static void RENDER_WORLD_NORMAL_DIFFUSE(
			ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
			NScene::Scene & scene,
			RenderTexture & targetWorld,
			RenderTexture & targetNormal,

			RenderTexture & targetDiffuse,

			RenderTexture & targetProperty,
			DepthTexture & depthWorld,
			DirectX::SimpleMath::Matrix worldMatrix);
		static void RENDER_WORLD(
			ID3D11Device * device, ID3D11DeviceContext *context, Asset& asset,
			NScene::Scene& scene,
			DirectX::SimpleMath::Matrix& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
			RenderTexture &renderTexture, DepthTexture& depthTexture);//rendering the normal scene


		static void RENDER_DIRECT_LIGHT(
			ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
			RenderTexture & renderTexture, DepthTexture & depthTexture,
			XMMATRIX orthoMVP,
			Vector3 eyePos, Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter,
			std::shared_ptr<RenderTexture> lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV,
			RenderTexture &textureWorld, RenderTexture &textureNormal, RenderTexture &textureDiffuse);

	};
}