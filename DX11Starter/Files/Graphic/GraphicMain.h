#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "SimpleShader.h"
#include "RenderTexture.h"
#include "DepthTexture.h"
#include "Graphic\Light.h"
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


namespace NGraphic {
	
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct ReflectiveShadowMap{
		std::shared_ptr<RenderTexture>	flux;
		std::shared_ptr<RenderTexture>	fluxEye;
		std::shared_ptr<RenderTexture>	normal;
		std::shared_ptr<DepthTexture>	depth;

	};
	struct MeshLoadInformation {
		KEnum id;
		char* path;
	};
	struct TextureLoadInformation {
		KEnum id;
		LPCWSTR path;
	};

	class GraphicMain {
	private:
		RenderStateStack m_renderStackStack;
		int m_rsm_flux_eye_perspective_width, m_rsm_flux_eye_perspective_height;
		DirectX::XMMATRIX getOrthogonalMatrixProj();
		DirectX::XMMATRIX getOrthogonalMatrixView();
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);


		bool initTextures		(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight);
		
		
	protected:
		ID3D11BlendState 
			*m_blendStateNoBlack,
			*m_blendStateTransparent;
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(NScene::Camera cam);// = 0;
		void beginRendering(ID3D11DeviceContext *context);// = 0;
		void endRendering(ID3D11DeviceContext *context);// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		int m_width, m_height;
		Mesh * mesh00;


		std::map<KEnum, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<KEnum, std::shared_ptr<DepthTexture>>	m_depthTextures;

		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int textureWidth, int textureHeight, int textureIndirectLightWidth, int textureIndirectLightHeight);
		
		void render(
			ID3D11Device * device, ID3D11DeviceContext *context, 
			ID3D11RenderTargetView* target, ID3D11DepthStencilView* targetDepth, D3D11_VIEWPORT& viewport,
			Asset& asset, NScene::Scene& scene);
	};
}