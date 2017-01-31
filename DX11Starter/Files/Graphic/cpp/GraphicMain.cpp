#include "Graphic\GraphicMain.h"
#include <string>

using namespace NGraphic;

static int SIZE_LIGHT_TEXTURE = 512*2;
float GraphicMain::RATIO_LIGHT_INNER = 0.5f;
void GraphicMain::processObject(NScene::Object obj) {
}


void GraphicMain::processCamera(NScene::Camera cam)
{
}

void GraphicMain::beginRendering(ID3D11DeviceContext* context)
{
	m_renderStackStack.save(context);
}

void GraphicMain::endRendering(ID3D11DeviceContext* context)
{
	m_renderStackStack.load(context);
	// Reset the states!
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
	context->OMSetBlendState(0, 0, 0xffffffff);

}

void GraphicMain::getScreenWidth(int & w, int & h)
{
}


DirectX::XMMATRIX NGraphic::GraphicMain::getOrthogonalMatrixProj()
{
	return DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
}

DirectX::XMMATRIX NGraphic::GraphicMain::getOrthogonalMatrixView()
{
	auto pos = Vector3(0, 0, 10);
	auto dir = Vector3(0, 0, -1);
	return DirectX::XMMatrixLookToLH(Vector3(0, 0, 10), Vector3(0, 0, -1), Vector3::Up);
}

void GraphicMain::rendering(NScene::Scene scene)
{

	
}
LightInfo NGraphic::GraphicMain::getLightInfo(ID3D11Device *device)
{
	LightInfo info{ std::shared_ptr<RenderTexture>(new RenderTexture()),std::shared_ptr<DepthTexture>(new DepthTexture) };
	info.position->init(device, m_width, m_height);
	info.depth->init(device, m_width, m_height);
	return info;
}
bool GraphicMain::initTextures(ID3D11Device * device, ID3D11DeviceContext * context,
	int width, int height,
	int textureIndirectLightWidth, int textureIndirectLightHeight)
{
#define INIT_DEPTH_TEXTURE(key,defWidth, defHeight) \
m_depthTextures[key] = std::shared_ptr<DepthTexture>(new DepthTexture());\
m_depthTextures[key]->init(device, defWidth, defHeight);
#define INIT_RENDER_TEXTURE(key,defWidth, defHeight) \
this->m_renderTextures[key]	= std::shared_ptr<RenderTexture>(new RenderTexture());\
this->m_renderTextures[key]	->init(device, defWidth, defHeight);
	std::cout << "initTextures";
	
	INIT_RENDER_TEXTURE(TARGET_WORLD,			width, height);
	INIT_RENDER_TEXTURE(TARGET_NORMAL,			width, height);
	INIT_RENDER_TEXTURE(TARGET_DIFFUSE,			width, height);
	INIT_RENDER_TEXTURE(TARGET_PROPERTY,		width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_FRONT,width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_BACK, width, height);
	INIT_RENDER_TEXTURE(TARGET_FINAL,			width, height);

	INIT_DEPTH_TEXTURE(DEPTH_WORLD, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_FINAL, width, height);
	INIT_DEPTH_TEXTURE(TARGET_LIGHTSHAFT_FRONT, width, height);
	INIT_DEPTH_TEXTURE(TARGET_LIGHTSHAFT_BACK, width, height);
	//m_depthTextures[TARGET_LIGHTSHAFT_BACK] = m_depthTextures[TARGET_LIGHTSHAFT_FRONT];

	return true;
}

GraphicMain::GraphicMain()
{
}
bool GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, 
	int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight)
{
	this->m_width = width;
	this->m_height = height;
	m_rsm_flux_eye_perspective_width = textureIndirectLightWidth;
	m_rsm_flux_eye_perspective_height = textureIndirectLightHeight;

	if (
		!initTextures(device,context,width,height, textureIndirectLightWidth, textureIndirectLightHeight)
		
		) return false;
	D3D11_BLEND_DESC noBlack = {};
	noBlack.RenderTarget[0].BlendEnable = true;
	noBlack.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	noBlack.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
	noBlack.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noBlack.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	noBlack.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	noBlack.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noBlack.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&noBlack, &m_blendStateNoBlack);

	D3D11_BLEND_DESC transparent = {};
	transparent.RenderTarget[0].BlendEnable = true;
	transparent.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparent.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparent.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&transparent, &m_blendStateTransparent);

	orthoView = DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up);
	orthoMVP = DirectX::XMMatrixMultiply( 
		DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up) , 
		DirectX::XMMatrixOrthographicLH(1,1,0.1,100) );

	return true;
}

void NGraphic::GraphicMain::renderWorldNormalDiffuse(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, 
	DirectX::SimpleMath::Matrix worldMatrix)
{
	beginRendering(context);
	DirectX::XMFLOAT4X4 matrixStore;
	SimpleVertexShader&		shaderVert	= *asset.m_shadersVert[RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY];
	SimpleFragmentShader&	shaderFrag	= *asset.m_shadersFrag[RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY];
	
	m_renderTextures[TARGET_WORLD]	->clear(context, 0, 0, 0, 0);
	m_renderTextures[TARGET_NORMAL]	->clear(context, 0, 0, 0, 0);
	m_renderTextures[TARGET_DIFFUSE]->clear(context, 0, 0, 0, 0);
	m_renderTextures[TARGET_PROPERTY]->clear(context, 0, 0, 0,0);
	m_depthTextures[DEPTH_WORLD]	->clear(context);
	ID3D11RenderTargetView *renderTargets[]{
		m_renderTextures[TARGET_WORLD]->getRenderTargetView(),
		m_renderTextures[TARGET_NORMAL]->getRenderTargetView(),
		m_renderTextures[TARGET_DIFFUSE]->getRenderTargetView(),
		m_renderTextures[TARGET_PROPERTY]->getRenderTargetView()
	};
	context->OMSetRenderTargets(4, renderTargets, m_depthTextures[DEPTH_WORLD]->getDepthStencilView());
	context->RSSetViewports(1, &m_renderTextures[TARGET_WORLD]->getViewPort() );

	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matrixStore);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(m_renderTextures[TARGET_WORLD]->getWidth(), m_renderTextures[TARGET_WORLD]->getHeight()))); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matrixStore);
	shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++) {
		NGraphic::NScene::Object& object = **it;
		NGraphic::Mesh& mesh = *asset.m_meshes[(*it)->m_meshId];
		auto modelWorldMatrix = DirectX::XMMatrixMultiply(worldMatrix, object.getModelMatrix());
		DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(object.getModelMatrix()) ); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matrixStore);
		shaderFrag.SetShaderResourceView("texture_diffuse", asset.m_textures[object.m_textures[TEXTURE_TYPE_DIFFUSE]]);
		shaderFrag.SetShaderResourceView("texture_normal", asset.m_textures[object.m_textures[TEXTURE_TYPE_NORMAL]]);
		shaderFrag.SetShaderResourceView("texture_property", asset.m_textures[object.m_textures[TEXTURE_TYPE_PROPERTY]]);
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
	}
	//clean up bindings
	shaderFrag.SetShaderResourceView("texture_diffuse", 0);
	shaderFrag.SetShaderResourceView("texture_normal", 0);
	shaderFrag.SetShaderResourceView("texture_property", 0);
	endRendering(context);
}

void NGraphic::GraphicMain::renderUI(ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, DirectX::SimpleMath::Matrix worldMatrix)
{
	beginRendering(context);
	endRendering(context);
}



void NGraphic::GraphicMain::renderWorld(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, 
	DirectX::SimpleMath::Matrix& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
	RenderTexture& renderTexture, DepthTexture& depthTexture)
{
#define RENDER_OBJS for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++) {\
		NGraphic::NScene::Object& light = **it; \
		NGraphic::Mesh& mesh = *asset.m_meshes[light.m_meshId]; \
		DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(light.getModelMatrix())); \
		shaderVert.SetMatrix4x4("world", matrixStore); \
		shaderVert.CopyAllBufferData(); \
		shaderFrag.CopyAllBufferData(); \
		UINT stride = sizeof(Vertex); \
		UINT offset = 0; \
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset); \
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0); \
		context->DrawIndexed(\
			mesh.getBufferIndexCount(), \
			0, \
			0); \
	}

	DirectX::XMFLOAT4X4 matrixStore;
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_WORLD];
	renderTexture.clear(context, 0, 0, 0, 99999);
	depthTexture.clear(context);
	//render the front face first
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(viewMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matrixStore);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matrixStore);
	//shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	RENDER_OBJS;


	endRendering(context);

}

void NGraphic::GraphicMain::renderFrustum(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
	Vector3 eyePosition,
	DirectX::XMMATRIX& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
	RenderTexture& renderTexture, DepthTexture& depthTexture,
	RenderTexture& renderTexture2, DepthTexture& depthTexture2) {
#define RENDER_LIGHTS(shaderVertSimpleColor,DshaderFrag) 	{NGraphic::Mesh& mesh = *asset.m_meshes[MESH_POINTLIGHT];\
DirectX::XMStoreFloat4x4(&matWorld, XMMatrixTranspose( worldMatrix));\
shaderVertSimpleColor.SetMatrix4x4("world", matWorld);\
shaderVertSimpleColor.CopyAllBufferData();\
DshaderFrag.CopyAllBufferData();\
UINT stride = sizeof(Vertex);\
UINT offset = 0;\
context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);\
context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);\
context->DrawIndexed(\
	mesh.getBufferIndexCount(),\
	0,\
	0);};

	beginRendering(context);
	DirectX::XMFLOAT4X4 matWorld,matView,matProj;
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_WORLD];
	SimpleVertexShader&		shaderVertSimpleColor = *asset.m_shadersVert[RENDER_FRUSTUM_INSIDE];
	SimpleFragmentShader&	shaderFragSimpleColor = *asset.m_shadersFrag[RENDER_FRUSTUM_INSIDE];
	renderTexture.clear(context, 0, 0, 0, 0);
	depthTexture.clear(context);
	//render the front face first
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	DirectX::XMStoreFloat4x4(&matView, XMMatrixTranspose(viewMatrix)); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&matProj, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!

	shaderVert.SetMatrix4x4("view", matView);
	shaderVert.SetMatrix4x4("proj", matProj);
	//shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);
	shaderVert.SetShader();
	shaderFrag.SetShader();
	RENDER_LIGHTS(shaderVert,shaderFrag);

	context->RSSetState(asset.RASTR_STATE_CULL_FRONT);
	renderTexture2.clear(context, 0, 0, 0, 0);
	depthTexture2.clear(context);
	renderTexture2.setRenderTarget(context, depthTexture2.getDepthStencilView());

	RENDER_LIGHTS(shaderVert, shaderFrag);
	//context->OMSetDepthStencilState(asset.DEPTH_STATE_SKYBOX, 0);
	{


		renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
		shaderVertSimpleColor.SetMatrix4x4("view", matView);
		shaderVertSimpleColor.SetMatrix4x4("proj", matProj);
		shaderFragSimpleColor.SetFloat3("eyePos",eyePosition);

		shaderVertSimpleColor.SetShader();
		shaderFragSimpleColor.SetShader();
		RENDER_LIGHTS(shaderVertSimpleColor, shaderFragSimpleColor);
	}

	endRendering(context);
}

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

void NGraphic::GraphicMain::renderDirectLight(
	ID3D11Device * device, ID3D11DeviceContext * context,
	RenderTexture& renderTexture, DepthTexture& depthTexture,
	Asset& asset, 
	Vector3 eyePos,
	Vector3 lightPos,Vector3 lightDir, Vector4 lightColor,float lightInner, float lightOutter,
	std::shared_ptr<RenderTexture> lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV)
{
	beginRendering(context);
	DirectX::XMFLOAT4X4 matrixStore;
	NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_PLANE];
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_DIRECT_LIGHT];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_DIRECT_LIGHT];
	//renderTexture.clear(context, 0, 0, 0, 1);
	//depthTexture.clear(context);
	context->OMSetBlendState(asset.BLEND_STATE_ADDITIVE, 0, 0xffffffff);
	context->RSSetState(asset.RASTR_STATE_CULL_BACK);
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(orthoMVP)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("matMVP", matrixStore);
	shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);
	shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
	shaderFrag.SetShaderResourceView("textureWorld", m_renderTextures[TARGET_WORLD]->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureNormal", m_renderTextures[TARGET_NORMAL]->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureDiffuse", m_renderTextures[TARGET_DIFFUSE]->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureShadow", lightShadow->getShaderResourceView());
	shaderFrag.SetFloat3("eyePos", eyePos);
	shaderFrag.SetFloat3("lightPos", lightPos);
	shaderFrag.SetFloat3("lightColor", Vector3(lightColor));
	shaderFrag.SetFloat3("lightDir", lightDir);
	shaderFrag.SetFloat("lightInner", lightInner);
	shaderFrag.SetFloat("lightOutter", lightOutter);
	shaderFrag.SetFloat("lightPower", lightColor.w);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(lightMVP)); // Transpose for HLSL!
	shaderFrag.SetMatrix4x4("matLightMVP", matrixStore);
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
	shaderFrag.SetShaderResourceView("textureWorld", 0);
	shaderFrag.SetShaderResourceView("textureNormal", 0);
	shaderFrag.SetShaderResourceView("textureDiffuse", 0);
	shaderFrag.SetShaderResourceView("textureShadow", 0);

	endRendering(context);
}
float density = 0.0;

void NGraphic::GraphicMain::renderLightShaft(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	RenderTexture& renderTexture, DepthTexture& depthTexture,
	Asset & asset, 
	Vector3 eyePos, Vector3 eyeLook, 
	Vector3 lightPos, Vector3 lightDir, Vector4 lightColor, float lightInner, float lightOutter,
	std::shared_ptr<RenderTexture> frustumFront, std::shared_ptr<RenderTexture> frustumBack, std::shared_ptr<RenderTexture> lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV)
{
	beginRendering(context);

	context->OMSetBlendState(asset.BLEND_STATE_ADDITIVE, 0, 0xffffffff);
	context->RSSetState(asset.RASTR_STATE_CULL_BACK);

	DirectX::XMFLOAT4X4 matrixStore;
	NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_PLANE];
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_LIGHT_SHAFT];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_LIGHT_SHAFT];

	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	depthTexture.clear(context);

	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(orthoMVP)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("matMVP", matrixStore);
	shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);
	shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
	shaderFrag.SetShaderResourceView("textureShadow", lightShadow->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureWorld", m_renderTextures[TARGET_WORLD]->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureFrustumFront", frustumFront->getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureFrustumBack", frustumBack->getShaderResourceView());
	shaderFrag.SetFloat("density", density);
	shaderFrag.SetFloat3("eyePos", eyePos);
	shaderFrag.SetFloat3("eyeLook", eyeLook);
	shaderFrag.SetFloat3("lightPos", lightPos);
	shaderFrag.SetFloat3("lightDir", lightDir);
	shaderFrag.SetFloat3("lightColor", Vector3(lightColor));
	shaderFrag.SetFloat("lightPower", lightColor.w);
	shaderFrag.SetFloat("lightInner",  lightInner);
	shaderFrag.SetFloat("lightOutter", lightOutter);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(lightMVP)); // Transpose for HLSL!
	shaderFrag.SetMatrix4x4("matLightMVP", matrixStore);
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
void NGraphic::GraphicMain::renderDebug(
	ID3D11Device * device, ID3D11DeviceContext * context,
	RenderTexture& renderTexture, DepthTexture& depthTexture, 
	
	Asset& asset, NGame::Context game, NScene::Scene& scene) {
	beginRendering(context);
	DirectX::XMFLOAT4X4 matStore;
	auto &shaderVert = *asset.m_shadersVert[RENDER_TRANSPARENT];
	auto &shaderFrag = *asset.m_shadersFrag[RENDER_TRANSPARENT];
	NGraphic::Mesh&		sphere = *asset.m_meshes[MESH_ID_SPHERE];
	NGraphic::Mesh&		box = *asset.m_meshes[MESH_ID_CUBE];
	NGraphic::MeshLine&	line = *asset.m_meshLine;
	NGraphic::MeshCube&	cube = *asset.m_meshCube;
	auto r = renderTexture.getRenderTargetView();
	auto &view = renderTexture.getViewPort();
	//atuo d = depthTexture.getDepthStencilView();
	context->OMSetRenderTargets(1, &r, NULL);
	context->RSSetViewports(1, &view);
	


	//context->RSSetState(asset.RASTR_STATE_CULL_);
	context->OMSetBlendState(asset.BLEND_STATE_ADDITIVE, 0, 0xffffffff);

	DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matStore);
	DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matStore);
	shaderFrag.SetShaderResourceView("textureEyeDepth", m_depthTextures[DEPTH_WORLD]->getShaderResourceView());
	shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
	shaderFrag.SetFloat("SCREEN_WIDTH", renderTexture.getWidth());
	shaderFrag.SetFloat("SCREEN_HEIGHT", renderTexture.getHeight() );
	shaderVert.SetShader();
	shaderFrag.SetShader();

	
	if (false)for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		
		Vector3 lightScale = it->get()->m_scale;
		it->get()->setScale(Vector3::One);

		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose((**it).getModelMatrix())); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat3("color", Vector3((**it).m_lightColor));


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &sphere.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(sphere.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			sphere.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		it->get()->setScale(lightScale);
	}
	float red = 0;
	float angle = -game.frustum.m_angle/2;
	float angleIncrease = game.frustum.m_angle / game.frustum.m_division;
	for (auto it = game.frustum.planesX.begin(); it != game.frustum.planesX.end(); it++) {
		auto matRotation = DirectX::XMMatrixRotationY( angle);
		auto matScale = DirectX::XMMatrixScaling(0.051, 0.5f, 20);
		red += 0.2f;


		//DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(matRotation,)); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(DirectX::XMMatrixMultiply(matScale, matRotation))); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat4("color", Vector4(red,1,1,1.0) );


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &box.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(box.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			box.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices

		angle += angleIncrease;
	
	}
	context->RSSetState(asset.RASTR_STATE_CULL_BACK);
	ID3D11Buffer * bufferVertices, *bufferIndices;
	
	if (false)for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		bufferVertices = cube.getBufferVertices();
		bufferIndices = cube.getBufferIndices();
		Vector3 lightScale = it->get()->m_scale;
		it->get()->setScale(Vector3::One);

		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose((**it).getModelMatrix())); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		Vector3 color = (**it).m_lightColor;
		shaderFrag.SetFloat4("color", Vector4(color.x, color.y, color.z,1));


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &bufferVertices, &stride, &offset);
		context->IASetIndexBuffer(bufferIndices, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			3*2*6,
			//cube.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		it->get()->setScale(lightScale);
	}

	DirectX::XMStoreFloat4x4(&matStore, Matrix::Identity); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("world", matStore);
	float randomSeed = 100;

	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	context->OMSetBlendState(asset.BLEND_STATE_TRANSPARENT, 0, 0xffffffff);
	for (auto it = asset.m_frustums.begin(); it != asset.m_frustums.end(); it++) {
		bufferVertices = it->second->getBufferVertices();
		bufferIndices = it->second->getBufferIndices();
		auto color = asset.getRandomColor(randomSeed++);
		shaderFrag.SetFloat4("color", Vector4(color.x,color.y,color.z,0.5f) );


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &bufferVertices, &stride, &offset);
		context->IASetIndexBuffer(bufferIndices, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			it->second->getBufferIndexCount(),
			//cube.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		
	}


	context->RSSetState(asset.RASTR_WIREFRAME);

	std::cout << line.getBufferIndexCount() << "\n";
	if(false)for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		bufferVertices = line.getBufferVertices();
		bufferIndices = line.getBufferIndices();
		Vector3 lightScale = it->get()->m_scale;
		it->get()->setScale(Vector3::One);

		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose((**it).getModelMatrix())); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat3("color", Vector3((**it).m_lightColor));


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &bufferVertices, &stride, &offset);
		context->IASetIndexBuffer(bufferIndices, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			line.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		it->get()->setScale(lightScale);
	}

	shaderFrag.SetShaderResourceView("textureEyeDepth", 0);

	
	endRendering(context);

}
void NGraphic::GraphicMain::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
	Asset& asset, NGame::Context &game	
	)
{
	NScene::Scene & scene = *game.m_scene;
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		NScene::Light& light = **it;
		//light.setRotation(light.m_rotation * Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), 0.000051f));
	}


	m_renderTextureDummy.setRenderTargetView(target, viewport);
	m_depthTextureDummy.setDepthStencilView(targetDepth);
	m_renderTextureDummy.clear(context, 0, 0, 0, 0);
	m_depthTextureDummy.clear(context);
	DirectX::SimpleMath::Matrix matSceneMvpFirstPerson = 
		scene.m_camMain.getProjectionMatrix(viewport.Width, viewport.Height) *
		scene.m_camMain.getViewMatrix() * scene.m_camMain.getModelMatrix();
	
	if(false){
		beginRendering(context);
		m_renderTextureDummy.clear(context, 0, 0, 0, 0);
		m_depthTextureDummy.clear(context);
		m_renderTextureDummy.setRenderTarget(context, m_depthTextureDummy.getDepthStencilView());
		//context->ClearDepthStencilView(targetDepth, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		auto skyVS = asset.m_shadersVert[RENDER_SKYBOX].get();
		auto skyPS = asset.m_shadersFrag[RENDER_SKYBOX].get();

		DirectX::XMFLOAT4X4  view, projection;

		//DirectX::XMStoreFloat4x4(&mvp, XMMatrixTranspose(matSceneMvpFirstPerson)); // Transpose for HLSL!
		//DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(viewport.Width, viewport.Height))); // Transpose for HLSL!


																																		// Set up shaders
		skyVS->SetMatrix4x4("view", view);	
		skyVS->SetMatrix4x4("projection", projection);
		skyVS->CopyAllBufferData();
		skyVS->SetShader();

		skyPS->SetShaderResourceView("Sky", asset.m_texturesCubeMap[TEXTURE_ID_SKYBOX_SUNNY]);
		skyPS->SetSamplerState("Sampler", asset.m_samplers[SAMPLER_ID_WRAP]);
		skyPS->CopyAllBufferData();
		skyPS->SetShader();

		// Set the proper render states

		// Set the proper render states
		context->RSSetState(asset.RASTR_STATE_CULL_FRONT);
		context->OMSetDepthStencilState(asset.DEPTH_STATE_SKYBOX, 0);

		// Actually draw
		NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_SPHERE];

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			mesh.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		endRendering(context);
	}

	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		if (m_lightInfos.find(it->get()->m_id) != m_lightInfos.end()) continue;
		m_lightInfos[it->get()->m_id] = getLightInfo(device);
	}
	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	auto worldMatrixFrustum = DirectX::SimpleMath::Matrix::CreateRotationX(3.14 / 2);
	auto viewMatirx = scene.m_camMain.getViewMatrix();
	auto projMatrix = scene.m_camMain.getProjectionMatrix(m_width, m_height);

	//context->OMSetBlendState(asset.BLEND_STATE_, 0, 0xffffffff);
	//context->RSSetState(asset.RASTR_STATE_CULL_BACK);

	renderWorldNormalDiffuse(device, context, asset, scene, worldMatrix);
	//now start rendering real stuff
	

	m_renderTextures[TARGET_FINAL]->clear(context, 0, 0, 0, 1);
	

	if (true) {
		m_depthTextures[DEPTH_FINAL]->clear(context);
		renderDebug(device, context, *m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
			 asset, game, scene);

	}
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		

		NScene::Light& light = **it;
		LightInfo& lightInfo = m_lightInfos[it->get()->m_id];
		//renderWorld(device, context, asset, scene, worldMatrix, lightInfo.position, lightInfo.depth);
		renderWorld(device, context, asset, scene,
			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix(lightInfo.position->getWidth(), lightInfo.position->getHeight()),
			*lightInfo.position, *lightInfo.depth);
		auto lightWorldMatirx = DirectX::XMMatrixMultiply(worldMatrixFrustum, it->get()->getModelMatrix());


		DirectX::XMMATRIX lightMVP = DirectX::XMMatrixMultiply(light.getViewMatrix(), light.getProjectionMatrix(lightInfo.position->getWidth(), lightInfo.position->getHeight()));
		if (true) {
			m_depthTextureDummy.clear(context);
			m_depthTextures[DEPTH_FINAL]->clear(context);
			renderDirectLight(
				device, context,
				*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
				//m_renderTextureDummy, m_depthTextureDummy,
				//m_renderTextureDummy, m_depthTextureDummy, 
				asset,
				scene.m_camMain.m_pos,
				light.m_pos, light.m_dirLook, light.m_lightColor, light.getFOV() * RATIO_LIGHT_INNER, light.getFOV(),
				m_lightInfos[it->get()->m_id].position, lightMVP, light.getFOV());
		}

		if (true) {
			renderFrustum(device, context, asset,
				scene.m_camMain.m_pos,
				lightWorldMatirx, viewMatirx, projMatrix,
				*m_renderTextures[TARGET_LIGHTSHAFT_FRONT], *m_depthTextures[TARGET_LIGHTSHAFT_FRONT],
				*m_renderTextures[TARGET_LIGHTSHAFT_BACK], *m_depthTextures[TARGET_LIGHTSHAFT_BACK]);
			renderLightShaft(
				device, context,
				//m_renderTextureDummy, m_depthTextureDummy,
				*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
				asset,
				scene.m_camMain.m_pos, scene.m_camMain.m_dirLook,
				light.m_pos, light.m_dirLook, light.m_lightColor, light.getFOV() * RATIO_LIGHT_INNER, light.getFOV(),
				m_renderTextures[TARGET_LIGHTSHAFT_FRONT],
				m_renderTextures[TARGET_LIGHTSHAFT_BACK],
				m_lightInfos[it->get()->m_id].position, lightMVP, light.getFOV());
		}

		if (false) {
			m_depthTextureDummy.clear(context);
			renderSkyboxReflection(device, context, m_renderTextureDummy, m_depthTextureDummy, asset,
				scene.m_camMain.m_pos,

				asset.m_texturesCubeMap[TEXTURE_ID_SKYBOX_SUNNY], m_renderTextures[TARGET_WORLD]->getShaderResourceView(), m_renderTextures[TARGET_NORMAL]->getShaderResourceView());

		}


		

		//m_depthTextureDummy.clear(context);
		//m_depthTextures[DEPTH_FINAL]->clear(context);
		//m_depthTextures[DEPTH_FINAL]->clear(context);
		
	}

	//let's render debug
	


	




}

