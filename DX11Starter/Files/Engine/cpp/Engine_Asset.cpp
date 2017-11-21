#include <Engine\Asset.h>
#include <iostream>
using namespace KEngine;
#define print(content) std::cout<<"Asset : "<<content<<"\n"

std::list<LoadInfoMesh> Asset::getLoadListMesh()
{
	std::list<LoadInfoMesh> lst({
		{ KEnum::MESH_SPONZA, "Resource/Mesh/sponza.obj" },
		{ KEnum::MESH_SPHERE, "Resource/Mesh/sphere.obj" },
		{ KEnum::MESH_CUBE, "Resource/Mesh/cube.obj" },
		{ KEnum::MESH_CONE, "Resource/Mesh/cone.obj" },
		{ KEnum::MESH_CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ KEnum::MESH_HELIX, "Resource/Mesh/helix.obj" },
		{ KEnum::MESH_TORUS, "Resource/Mesh/torus.obj" },
		{ KEnum::MESH_PLANE, "Resource/Mesh/plane.obj" },
		{ KEnum::MESH_FRUSTUM, "Resource/Mesh/frustum.obj" },
		{ KEnum::MESH_SPOTLIGHT, "Resource/Mesh/spotlight.obj" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderVert()
{
	std::list<LoadInfoShader> lst({
		LoadInfoShader(RENDER_SPHERICAL_HARMONIC_DIFFUSE_MAP,							L"Resource/Shader/Atlas/DiffuseCubeMapVS.hlsl"),
		LoadInfoShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM,			L"Resource/Shader/Atlas/SimpleLightVS.hlsl")
	});
	/*
		unused shader files
		LoadInfoShader(RENDER_SKYBOX,							L"Resource/Shader/SkyVS.hlsl"),
		LoadInfoShader(RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY,			L"Resource/Shader/defferedVS.hlsl"),
		LoadInfoShader(RENDER_WORLD,			L"Resource/Shader/worldSpaceVS.hlsl"),
		LoadInfoShader(RENDER_DIRECT_LIGHT,			L"Resource/Shader/directLightVS.hlsl"),
		LoadInfoShader(RENDER_LIGHT_SHAFT,			L"Resource/Shader/lightShaftVS.hlsl"),
		LoadInfoShader(RENDER_FRUSTUM_INSIDE,			L"Resource/Shader/frustumScreenSpaceVS.hlsl"),
		LoadInfoShader(RENDER_SKYBOX_REFLECTION,			L"Resource/Shader/skyboxReflectVS.hlsl"),
		LoadInfoShader(RENDER_ONE_COLOR,			L"Resource/Shader/OneColorVS.hlsl"),
		LoadInfoShader(RENDER_TRANSPARENT,			L"Resource/Shader/TransparentVS.hlsl"),
		LoadInfoShader(SHADER_SIMPLE,			L"Resource/Shader/SimpleShaderVS.hlsl")
	*/
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderFrag()
{
	std::list<LoadInfoShader> lst({
		LoadInfoShader(RENDER_SPHERICAL_HARMONIC_DIFFUSE_MAP,							L"Resource/Shader/Atlas/DiffuseCubeMapFS.hlsl"),
		LoadInfoShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM,			L"Resource/Shader/Atlas/SimpleLightFS.hlsl"
		,{
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			//(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			//(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)false, D3D11_USAGE_DEFAULT,0



		})
	});
	/*
		unused shader files
		LoadInfoShader(RENDER_SKYBOX,						L"Resource/Shader/SkyFS.hlsl"),
		LoadInfoShader(RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY,			L"Resource/Shader/defferedFS.hlsl"),
		LoadInfoShader(RENDER_WORLD,			L"Resource/Shader/worldSpaceFS.hlsl"),
		LoadInfoShader(RENDER_DIRECT_LIGHT,			L"Resource/Shader/directLightFS.hlsl"),
		LoadInfoShader(RENDER_LIGHT_SHAFT,			L"Resource/Shader/lightShaftFS.hlsl"),
		LoadInfoShader(RENDER_FRUSTUM_INSIDE,			L"Resource/Shader/frustumScreenSpaceFS.hlsl"),
		LoadInfoShader(RENDER_SKYBOX_REFLECTION,			L"Resource/Shader/skyboxReflectFS.hlsl"),
		LoadInfoShader(RENDER_ONE_COLOR,			L"Resource/Shader/OneColorFS.hlsl"),
		LoadInfoShader(RENDER_TRANSPARENT,			L"Resource/Shader/TransparentFS.hlsl"),
		LoadInfoShader(SHADER_SIMPLE,			L"Resource/Shader/SimpleShaderFS.hlsl"),
	*/
	return lst;
}

std::list<LoadInfoTexture> Asset::getLoadListTexture()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_DEFAULT,		L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_WHITE,		L"Resource/Texture/white.png" },
		{ TEXTURE_RED,		L"Resource/Texture/red.png" },
		{ TEXTURE_NORMAL_DEFAULT,		L"Resource/Texture/normal_default.jpg" },
		{ TEXTURE_PROBE0,		L"Resource/Texture/Probe0.png" },
	});
	return lst;
}

std::list<LoadInfoTexture> Asset::getLoadListTextureCubeMap()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_SKYBOX_SUNNY,		L"Resource/Texture/CubeMap/Sunny.dds" }
	});
	return lst;
}

bool Asset::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	//initialize debugging tools
	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();
	auto dataTextureCubeMap = getLoadListTextureCubeMap();


	if (!initShaders(device, context, getLoadListShaderVert(), getLoadListShaderFrag())) {
		print("init shader fail");
		return false;
	}
	else
		print("init shader succeess");
	
	if (!initMeshes(device, dataMesh)) {
		print("init meshes fail");
		return false;
	}
	else
		print("init meshes succeess");

	D3D11_RASTERIZER_DESC rsDescBack = {};
	rsDescBack.FillMode = D3D11_FILL_SOLID;
	rsDescBack.CullMode = D3D11_CULL_BACK;
	//rsDescBack.FrontCounterClockwise = true;
	rsDescBack.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDescBack, &m_rasterizers[KEnum::RASTR_CULLBACKFACE]);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &m_sampler);
	
}

bool Asset::initShaders(ID3D11Device * device, ID3D11DeviceContext * context, std::list<LoadInfoShader> dataVert, std::list<LoadInfoShader> dataFrag)
{

	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert.insert({ it->type , SimpleVertexShader(device, context) });
		auto& shaderVert = m_shadersVert.find(it->type)->second;
		
		if (!it->isCustomConstantBuffer)
		{
			if (!shaderVert.LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
		}
		else {
			if (!shaderVert.LoadShaderFileHLSLCustomConstantBuffer(it->path, "vs_5_0", it->customConstantBufferArguments))
				return false;
		}
		/*
		*/


	}
	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		m_shadersFrag.insert({ it->type, SimpleFragmentShader(device, context) });
		auto& shaderFrag = m_shadersFrag.find(it->type)->second;
		
		if (!it->isCustomConstantBuffer) {

			if (!shaderFrag.LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
		}
		else {
			if (!shaderFrag.LoadShaderFileHLSLCustomConstantBuffer(it->path, "ps_5_0", it->customConstantBufferArguments))
				return false;

		}
		
	}

	return true;
}
bool Asset::initMeshes(ID3D11Device* device, std::list<LoadInfoMesh> dataMesh) {
	for (auto it = dataMesh.begin(); it != dataMesh.end(); it++) {
		
		m_meshes.emplace(std::piecewise_construct, std::make_tuple(it->id), std::make_tuple(device, it->path,true));
		//(it->id,Mesh{device, it->path});
	}
	return true;
}
SimpleFragmentShader& Asset::getFragShader(KEnum name) {
	return m_shadersFrag.find(name)->second;
}

SimpleVertexShader& Asset::getVertShader(KEnum name) {
	return m_shadersVert.find(name)->second;

}
ID3D11RasterizerState * Asset::getRasterizer(KEnum name)
{
	return m_rasterizers.find(name)->second;
}