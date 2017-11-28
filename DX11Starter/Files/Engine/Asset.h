#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <list>
#include <string>
#include <map>
#include <Engine\Enum.h>
#include <Engine\SimpleShader.h>
#include <Engine\Mesh.h>
using namespace DirectX::SimpleMath;
namespace KEngine {
	struct LoadInfoMesh {
		KEnum id;
		char* path;
	};
	struct LoadInfoShader {
		KEnum type;
		LPCWSTR path;
		bool isCustomConstantBuffer;
		std::list<int> customConstantBufferArguments;
		LoadInfoShader(KEnum type, LPCWSTR path) {
			this->type = type;
			this->path = path;
			isCustomConstantBuffer = false;
		}
		LoadInfoShader(KEnum type, LPCWSTR path, std::initializer_list<int> args) {
			this->type = type;
			this->path = path;
			isCustomConstantBuffer = true;
			for (auto arg : args)
				customConstantBufferArguments.push_back(arg);
		}
	};
	struct LoadInfoTexture {
		KEnum id;
		LPCWSTR path;
	};
	class Asset {
	private:
		//bank
		std::map<KEnum, SimpleFragmentShader> m_shadersFrag;
		std::map<KEnum, SimpleVertexShader> m_shadersVert;
		std::map<KEnum, ID3D11ShaderResourceView*> m_textures;
		std::map<KEnum, ID3D11ShaderResourceView*> m_texturesCubeMap;
		std::map<KEnum, ID3D11SamplerState*> m_samplers; //sampler ID
		std::map<KEnum, ID3D11RasterizerState*> m_rasterizers;
		std::map<KEnum, ID3D11BlendState*> m_blendStates;

		std::list<LoadInfoShader>	getLoadListShaderVert();
		std::list<LoadInfoShader>	getLoadListShaderFrag();
		std::list<LoadInfoMesh>		getLoadListMesh();
		std::list<LoadInfoTexture>	getLoadListTexture();
		std::list<LoadInfoTexture>	getLoadListTextureCubeMap();
		bool initShaders(ID3D11Device* device, ID3D11DeviceContext *context, std::list<LoadInfoShader> vertInfo, std::list<LoadInfoShader> fragInfo);
		bool initMeshes(ID3D11Device* device, std::list<LoadInfoMesh> dataMesh);
	public:

		ID3D11SamplerState* m_sampler;
		std::map<KEnum, Mesh> m_meshes;
		ID3D11ShaderResourceView * getTexture(KEnum id);
		//ID3D11BlendState * BLEND_STATE_ADDITIVE, *BLEND_STATE_TRANSPARENT;
		//ID3D11RasterizerState
		//	* RASTR_STATE_CULL_FRONT,
		//	*RASTR_STATE_CULL_BACK,
		//	*RASTR_STATE_CULL_NONE,
		//	*RASTR_WIREFRAME;
		ID3D11DepthStencilState *	DEPTH_STATE_SKYBOX;

		bool init(ID3D11Device* device, ID3D11DeviceContext *context);
		SimpleFragmentShader&		getFragShader(KEnum name);
		SimpleVertexShader&			getVertShader(KEnum name);
		ID3D11RasterizerState*			getRasterizer(KEnum name);
		//Vector3 getRandomColor(int seed);

		/*
		Load frustums from game class for debugging purpose
		*/
		//std::map<int, std::shared_ptr< MeshCube >> m_frustums;
		//void loadDebug_frustums(ID3D11Device * device, std::vector<NGraphic::NFrustum::ClusterAABB> cubes);
	};
}