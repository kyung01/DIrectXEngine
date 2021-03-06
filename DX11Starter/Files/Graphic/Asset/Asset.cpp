#include <Graphic\Asset\Asset.h>
#include <DDSTextureLoader.h>
#include "..\..\Engine\Asset.h"
using namespace NGraphic;
std::list<LoadInfoMesh> Asset::getLoadListMesh()
{
	std::list<LoadInfoMesh> lst({
		{ KEnum::MESH_ID_SPONZA, "Resource/Mesh/sponza.obj" },
		{ KEnum::MESH_ID_SPHERE, "Resource/Mesh/sphere.obj" },
		{ KEnum::MESH_ID_CUBE, "Resource/Mesh/cube.obj" },
		{ KEnum::MESH_ID_CONE, "Resource/Mesh/cone.obj" },
		{ KEnum::MESH_ID_CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ KEnum::MESH_ID_HELIX, "Resource/Mesh/helix.obj" },
		{ KEnum::MESH_ID_TORUS, "Resource/Mesh/torus.obj" },
		{ KEnum::MESH_ID_PLANE, "Resource/Mesh/plane.obj" },
		{ KEnum::MESH_ID_FRUSTUM, "Resource/Mesh/frustum.obj" },
		{ KEnum::MESH_SPOTLIGHT, "Resource/Mesh/spotlight.obj" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderVert()
{
	std::list<LoadInfoShader> lst({
		LoadInfoShader(RENDER_SPHERICAL_HARMONIC_DIFFUSE_MAP,							L"Resource/Shader/Atlas/DiffuseCubeMapVS.hlsl"),
		LoadInfoShader(RENDER_SKYBOX,							L"Resource/Shader/SkyVS.hlsl"),
		LoadInfoShader( RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY,			L"Resource/Shader/defferedVS.hlsl" ),
		LoadInfoShader( RENDER_WORLD,			L"Resource/Shader/worldSpaceVS.hlsl" ),
		LoadInfoShader( RENDER_DIRECT_LIGHT,			L"Resource/Shader/directLightVS.hlsl" ),
		LoadInfoShader( RENDER_LIGHT_SHAFT,			L"Resource/Shader/lightShaftVS.hlsl" ),
		LoadInfoShader( RENDER_FRUSTUM_INSIDE,			L"Resource/Shader/frustumScreenSpaceVS.hlsl" ),
		LoadInfoShader( RENDER_SKYBOX_REFLECTION,			L"Resource/Shader/skyboxReflectVS.hlsl" ),
		LoadInfoShader( RENDER_ONE_COLOR,			L"Resource/Shader/OneColorVS.hlsl" ),
		LoadInfoShader( RENDER_TRANSPARENT,			L"Resource/Shader/TransparentVS.hlsl" ),
		LoadInfoShader( RENDER_TEST,			L"Resource/Shader/Atlas/SimpleLightVS.hlsl")
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderFrag()
{
	std::list<LoadInfoShader> lst({
		LoadInfoShader(RENDER_SPHERICAL_HARMONIC_DIFFUSE_MAP,							L"Resource/Shader/Atlas/DiffuseCubeMapFS.hlsl"),
		LoadInfoShader( RENDER_SKYBOX,						L"Resource/Shader/SkyFS.hlsl" ),
		LoadInfoShader( RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY,			L"Resource/Shader/defferedFS.hlsl" ),
		LoadInfoShader( RENDER_WORLD,			L"Resource/Shader/worldSpaceFS.hlsl" ),
		LoadInfoShader( RENDER_DIRECT_LIGHT,			L"Resource/Shader/directLightFS.hlsl" ),
		LoadInfoShader( RENDER_LIGHT_SHAFT,			L"Resource/Shader/lightShaftFS.hlsl" ),
		LoadInfoShader( RENDER_FRUSTUM_INSIDE,			L"Resource/Shader/frustumScreenSpaceFS.hlsl" ),
		LoadInfoShader( RENDER_SKYBOX_REFLECTION,			L"Resource/Shader/skyboxReflectFS.hlsl" ),
		LoadInfoShader( RENDER_ONE_COLOR,			L"Resource/Shader/OneColorFS.hlsl"),
		LoadInfoShader( RENDER_TRANSPARENT,			L"Resource/Shader/TransparentFS.hlsl"),
		LoadInfoShader( RENDER_TEST,			L"Resource/Shader/Atlas/SimpleLightFS.hlsl" 
		,{
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			//(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			//(int)true, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE,
			(int)false, D3D11_USAGE_DEFAULT,0

		
		
		})
	});
	return lst;
}

std::list<LoadInfoTexture> Asset::getLoadListTexture()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_ID_DEFAULT,		L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_ID_WHITE,		L"Resource/Texture/white.png" },
		{ TEXTURE_ID_RED,		L"Resource/Texture/red.png" },
		{ TEXTURE_ID_NORMAL_DEFAULT,		L"Resource/Texture/normal_default.jpg" },
		{ TEXTURE_ID_PROBE0,		L"Resource/Texture/Probe0.png" },
	});
	return lst;
}

std::list<LoadInfoTexture> NGraphic::Asset::getLoadListTextureCubeMap()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_ID_SKYBOX_SUNNY,		L"Resource/Texture/CubeMap/Sunny.dds" }
	});
	return lst;
}

bool Asset::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	//initialize debugging tools
	m_meshLine = std::make_shared<NGraphic::MeshLine>(device);
	m_meshCube = std::make_shared<NGraphic::MeshCube>(device);
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();
	auto dataTextureCubeMap = getLoadListTextureCubeMap();

	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert[it->type] = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device, context));
		if (!it->isCustomConstantBuffer)
		{
			if (!m_shadersVert[it->type]->LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
		}
		else {
			if (!m_shadersVert[it->type]->LoadShaderFileHLSLCustomConstantBuffer(it->path, "vs_5_0", it->customConstantBufferArguments))
				return false;
		}

		
	}
	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		m_shadersFrag[it->type] = std::shared_ptr<SimpleFragmentShader>(new SimpleFragmentShader(device, context));
		if (!it->isCustomConstantBuffer) {

			if( !m_shadersFrag[it->type]->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
		}
		else {
			if (!m_shadersFrag[it->type]->LoadShaderFileHLSLCustomConstantBuffer(it->path, "ps_5_0", it->customConstantBufferArguments))
				return false;

		}
	}
	for (auto it = dataMesh.begin(); it != dataMesh.end(); it++) {
		auto mesh = new Mesh(device, it->path);
		m_meshes[it->id] = std::shared_ptr<Mesh>(mesh);
	}
	for (auto it = dataTexture.begin(); it != dataTexture.end(); it++) {
		ID3D11ShaderResourceView *texture;
		//DirectX::CreateWICTextureFromFileEx(device,)

		DirectX::CreateWICTextureFromFile(device, context, it->path, 0, &texture);
		m_textures[it->id] = texture;
	}
	for (auto it = dataTextureCubeMap.begin(); it != dataTextureCubeMap.end(); it++) {
		ID3D11ShaderResourceView *texture;
		DirectX::CreateDDSTextureFromFile(device, context, it->path, 0, &texture);
		m_texturesCubeMap[it->id] = texture;
	}

	D3D11_SAMPLER_DESC samplerDescPOINT = {};
	samplerDescPOINT.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Could be anisotropic
	samplerDescPOINT.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDescPOINT.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescPOINT.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescPOINT.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescPOINT.MaxAnisotropy = 16;
	samplerDescPOINT.MinLOD = 0;
	samplerDescPOINT.MaxLOD = D3D11_FLOAT32_MAX;
	D3D11_SAMPLER_DESC samplerDescWrap = {};
	samplerDescWrap.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Could be anisotropic
	samplerDescWrap.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDescWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.MaxAnisotropy = 16;
	samplerDescWrap.MinLOD = 0;
	samplerDescWrap.MaxLOD = D3D11_FLOAT32_MAX;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	D3D11_SAMPLER_DESC sampler_IDontKnow = {};
	sampler_IDontKnow.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Could be anisotropic
	sampler_IDontKnow.ComparisonFunc = D3D11_COMPARISON_LESS;
	sampler_IDontKnow.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.MaxAnisotropy = 16;
	sampler_IDontKnow.MinLOD = 0;
	sampler_IDontKnow.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC samplerDescLight = {};
	samplerDescLight.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescLight.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.BorderColor[0] = 1.0f;
	samplerDescLight.BorderColor[1] = 1.0f;
	samplerDescLight.BorderColor[2] = 1.0f;
	samplerDescLight.BorderColor[3] = 1.0f;
	samplerDescLight.MaxAnisotropy = 16;
	samplerDescLight.MinLOD = 0;
	samplerDescLight.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC samplerDescLightRMS = {};
	samplerDescLightRMS.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescLightRMS.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.BorderColor[0] = 0.0f;
	samplerDescLightRMS.BorderColor[1] = 0.0f;
	samplerDescLightRMS.BorderColor[2] = 0.0f;
	samplerDescLightRMS.BorderColor[3] = 0.0f;
	samplerDescLightRMS.MaxAnisotropy = 16;
	samplerDescLightRMS.MinLOD = 0;
	samplerDescLightRMS.MaxLOD = D3D11_FLOAT32_MAX;



	ID3D11SamplerState* samplerWrap,* samplerClamp, *samplerBorderOne,*samplerBorderZero,*samplerLinear,*samplerPoint;
	device->CreateSamplerState(&samplerDescWrap, &samplerWrap);
	device->CreateSamplerState(&samplerDesc, &samplerClamp);
	device->CreateSamplerState(&sampler_IDontKnow , &samplerLinear);
	device->CreateSamplerState(&samplerDescLight, &samplerBorderOne);
	device->CreateSamplerState(&samplerDescLightRMS, &samplerBorderZero);
	device->CreateSamplerState(&samplerDescPOINT, &samplerPoint);
	m_samplers[SAMPLER_ID_WRAP] = samplerWrap;
	m_samplers[SAMPLER_ID_CLAMP] = samplerClamp;
	m_samplers[SAMPLER_ID_BORDER_ONE] = samplerBorderOne;
	m_samplers[SAMPLER_ID_BORDER_ZERO] = samplerBorderZero;
	m_samplers[SAMPLER_ID_LINEAR] = samplerLinear;
	m_samplers[SAMPLER_ID_POINT] = samplerPoint;




	// Create a rasterizer state so we can render backfaces
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	//rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &RASTR_STATE_CULL_FRONT);
	D3D11_RASTERIZER_DESC rsWireframe = {};
	rsWireframe.FillMode = D3D11_FILL_WIREFRAME;
	rsWireframe.CullMode = D3D11_CULL_NONE;
	//rsWireframe.FrontCounterClockwise = true;
	rsWireframe.DepthClipEnable = true;
	device->CreateRasterizerState(&rsWireframe, &RASTR_WIREFRAME);
	D3D11_RASTERIZER_DESC rsDescBack = {};
	rsDescBack.FillMode = D3D11_FILL_SOLID;
	rsDescBack.CullMode = D3D11_CULL_BACK;
	//rsDescBack.FrontCounterClockwise = true;
	rsDescBack.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDescBack, &RASTR_STATE_CULL_BACK);

	D3D11_RASTERIZER_DESC rsCullNoneNoDepth = {};
	rsCullNoneNoDepth.FillMode = D3D11_FILL_SOLID;
	rsCullNoneNoDepth.CullMode = D3D11_CULL_NONE;
	//rsCullNoneNoDepth.FrontCounterClockwise = true;
	rsCullNoneNoDepth.DepthClipEnable = true;
	device->CreateRasterizerState(&rsCullNoneNoDepth, &RASTR_STATE_CULL_NONE);

	// Create a depth state so that we can accept pixels
	// at a depth less than or EQUAL TO an existing depth
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Make sure we can see the sky (at max depth)
	device->CreateDepthStencilState(&dsDesc, &DEPTH_STATE_SKYBOX);


	D3D11_BLEND_DESC additive = {};
	additive.RenderTarget[0].BlendEnable = true;
	additive.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	additive.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additive.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additive.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	additive.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	additive.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additive.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&additive, &BLEND_STATE_ADDITIVE);

	D3D11_BLEND_DESC transparent = {};
	transparent.RenderTarget[0].BlendEnable = true;
	transparent.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparent.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparent.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&transparent, &BLEND_STATE_TRANSPARENT);
}

Vector3* colors = new Vector3[8]{ Vector3(1,0,0),Vector3(1,0.5,0),Vector3(1,1,0) ,Vector3(0,1,0),Vector3(0.3,0.3,1),Vector3(0,0,1),Vector3(1,0,1),Vector3(0,1,1) };
Vector3 NGraphic::Asset::getRandomColor(int seed)
{
	return colors[(int)seed%9];
	//return Vector3();
}

void NGraphic::Asset::loadDebug_frustums(ID3D11Device * device, std::vector<NGraphic::NFrustum::ClusterAABB> cubes)
{
	for (int i = 0; i < cubes.size(); i++) {
		m_frustums[i] = std::make_shared<MeshCube>(device,
			cubes[i].a0, cubes[i].a1, cubes[i].a2, cubes[i].a3, cubes[i].b0, cubes[i].b1, cubes[i].b2, cubes[i].b3);
	}
}
