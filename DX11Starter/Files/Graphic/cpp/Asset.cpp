#include <Graphic\Asset\Asset.h>
#include <DDSTextureLoader.h>
using namespace NGraphic;

std::list<LoadInfoMesh> Asset::getLoadListMesh()
{
	std::list<LoadInfoMesh> lst({
		{ KEnum::MESH_ID_CONE, "Resource/Mesh/cone.obj" },
		{ KEnum::MESH_ID_CUBE, "Resource/Mesh/cube.obj" },
		{ KEnum::MESH_ID_CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ KEnum::MESH_ID_HELIX, "Resource/Mesh/helix.obj" },
		{ KEnum::MESH_ID_SPHERE, "Resource/Mesh/sphere.obj" },
		{ KEnum::MESH_ID_TORUS, "Resource/Mesh/torus.obj" },
		{ KEnum::MESH_ID_PLANE, "Resource/Mesh/plane.obj" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderVert()
{
	std::list<LoadInfoShader> lst({
		{ RENDER_SKYBOX,			L"Resource/Shader/SkyVS.hlsl" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderFrag()
{
	std::list<LoadInfoShader> lst({
		{ RENDER_SKYBOX,			L"Resource/Shader/SkyPS.hlsl" }
	});
	return lst;
}

std::list<LoadInfoTexture> Asset::getLoadListTexture()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_ID_DEFAULT,		L"Resource/Texture/textureTest00.jpg" }
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
	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();
	auto dataTextureCubeMap = getLoadListTextureCubeMap();

	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		m_shadersFrag[it->type] = std::shared_ptr<SimpleFragmentShader>(new SimpleFragmentShader(device, context));
		if (!m_shadersFrag[it->type]->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
	}
	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert[it->type] = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device, context));
		if (!m_shadersVert[it->type]->LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
	}
	for (auto it = dataMesh.begin(); it != dataMesh.end(); it++) {
		auto mesh = new Mesh(device, it->path);
		m_meshes[it->id] = std::make_unique<Mesh*>(mesh);
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
	samplerDescWrap.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
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

}
