#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <list>
#include <map>
#include <memory>
#include <Graphic\Asset\Mesh.h>
#include <Graphic\SimpleShader.h>
#include <Graphic\Enum.h>
//For debug purpose use buitin tools
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>


namespace NGraphic {
	struct LoadInfoMesh {
		KEnum id;
		char* path;
	};
	struct LoadInfoShader {
		KEnum type;
		LPCWSTR path;
	};
	struct LoadInfoTexture {
		KEnum id;
		LPCWSTR path;
	};
	class Asset {
	private:

		std::list<LoadInfoMesh>		getLoadListMesh();
		std::list<LoadInfoShader>	getLoadListShaderVert();
		std::list<LoadInfoShader>	getLoadListShaderFrag();
		std::list<LoadInfoTexture>	getLoadListTexture();
		std::list<LoadInfoTexture>	getLoadListTextureCubeMap();

	public:
		//Debug purpose.
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor> > m_primitiveBatch;
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

		std::map<KEnum, std::shared_ptr<SimpleFragmentShader>> m_shadersFrag;
		std::map<KEnum, std::shared_ptr<SimpleVertexShader>> m_shadersVert;
		std::map<KEnum, std::shared_ptr<Mesh>> m_meshes;
		std::map<KEnum, ID3D11ShaderResourceView*> m_textures;
		std::map<KEnum, ID3D11ShaderResourceView*> m_texturesCubeMap;
		std::map<KEnum, ID3D11SamplerState*> m_samplers; //sampler ID
		bool init(ID3D11Device* device, ID3D11DeviceContext *context);
		
		ID3D11BlendState * BLEND_STATE_ADDITIVE, *BLEND_STATE_TRANSPARENT;
		ID3D11RasterizerState 
			* RASTR_STATE_CULL_FRONT,
			* RASTR_STATE_CULL_NONE_NO_DEPTH;
		ID3D11DepthStencilState *	DEPTH_STATE_SKYBOX;
	};
 }