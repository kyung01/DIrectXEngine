#include "Graphic\GraphicMain.h"
#include <string>

using namespace NGraphic;

static int SIZE_LIGHT_TEXTURE = 512*2;
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
	
	INIT_RENDER_TEXTURE(TARGET_WORLD, width, height);
	INIT_RENDER_TEXTURE(TARGET_NORMAL, width, height);
	INIT_RENDER_TEXTURE(TARGET_DIFFUSE, width, height);
	INIT_RENDER_TEXTURE(TARGET_PROPERTY, width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_FRONT, width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_BACK, width, height);
	INIT_RENDER_TEXTURE(TARGET_FINAL, width, height);

	INIT_DEPTH_TEXTURE(DEPTH_WORLD, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_FINAL, width, height);
	INIT_DEPTH_TEXTURE(TARGET_LIGHTSHAFT_FRONT, width, height);
	m_depthTextures[TARGET_LIGHTSHAFT_BACK] = m_depthTextures[TARGET_LIGHTSHAFT_FRONT];

	return true;
}

GraphicMain::GraphicMain()
{
}
bool GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight)
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
	
	m_renderTextures[TARGET_WORLD]	->clear(context, 0, 0, 0, 1);
	m_renderTextures[TARGET_NORMAL]	->clear(context, 0, 0, 0, 1);
	m_renderTextures[TARGET_DIFFUSE]->clear(context, 0, 0, 0, 1);
	m_renderTextures[TARGET_PROPERTY]->clear(context, 0, 0, 0,1);
	m_depthTextures[DEPTH_WORLD]	->clear(context);
	ID3D11RenderTargetView *renderTargets[]{
		m_renderTextures[TARGET_WORLD]->getRenderTargetView(),
		m_renderTextures[TARGET_NORMAL]->getRenderTargetView(),
		m_renderTextures[TARGET_DIFFUSE]->getRenderTargetView(),
		m_renderTextures[TARGET_PROPERTY]->getRenderTargetView()
	};
	context->OMSetRenderTargets(4, renderTargets, m_depthTextures[DEPTH_WORLD]->getDepthStencilView());
	context->RSSetViewports(1, &m_renderTextures[TARGET_WORLD]->viewport);

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

				   /*
		*/
	}
	endRendering(context);
}

void NGraphic::GraphicMain::renderUI(ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, DirectX::SimpleMath::Matrix worldMatrix)
{
	beginRendering(context);
	endRendering(context);
}



void NGraphic::GraphicMain::renderWorld(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, int renderMode,
	DirectX::SimpleMath::Matrix& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
	std::shared_ptr<RenderTexture> renderTexture, std::shared_ptr<DepthTexture> depthTexture,
	std::shared_ptr<RenderTexture> renderTexture2, std::shared_ptr<DepthTexture> depthTexture2)
{
#define RENDER if(renderMode ==0){RENDER_OBJS} else if(renderMode == 1) {RENDER_LIGHTS}
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
#define RENDER_LIGHTS 	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {\
NGraphic::NScene::Object& light = **it;\
NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_FRUSTUM];\
auto newModelMatrix = DirectX::XMMatrixMultiply(worldMatrix,light.getModelMatrix() );\
DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(newModelMatrix));\
shaderVert.SetMatrix4x4("world", matrixStore);\
shaderVert.CopyAllBufferData();\
shaderFrag.CopyAllBufferData();\
UINT stride = sizeof(Vertex);\
UINT offset = 0;\
context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);\
context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);\
context->DrawIndexed(\
	mesh.getBufferIndexCount(),\
	0,\
	0);\
}
	beginRendering(context);

	DirectX::XMFLOAT4X4 matrixStore;
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_WORLD];
	renderTexture->clear(context, 0, 0, 0, 1);
	depthTexture->clear(context);
	//render the front face first
	renderTexture->setRenderTarget(context, depthTexture->getDepthStencilView());
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(viewMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matrixStore);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matrixStore);
	//shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	RENDER;
	if (renderTexture2 != 0 && depthTexture2 != 0) {
		renderTexture2->clear(context, 0, 0, 0, 1);
		depthTexture2->clear(context);
		renderTexture2->setRenderTarget(context, depthTexture2->getDepthStencilView());
		context->RSSetState(asset.RASTR_STATE_CULL_FRONT);
		RENDER;
	}

	endRendering(context);

}
void NGraphic::GraphicMain::renderFrustum(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, 
	DirectX::XMMATRIX& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
	std::shared_ptr<RenderTexture> renderTexture, std::shared_ptr<DepthTexture> depthTexture,
	std::shared_ptr<RenderTexture> renderTexture2, std::shared_ptr<DepthTexture> depthTexture2){
#define RENDER_LIGHTS 	NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_FRUSTUM];\
DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(worldMatrix));\
shaderVert.SetMatrix4x4("world", matrixStore);\
shaderVert.CopyAllBufferData();\
shaderFrag.CopyAllBufferData();\
UINT stride = sizeof(Vertex);\
UINT offset = 0;\
context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);\
context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);\
context->DrawIndexed(\
	mesh.getBufferIndexCount(),\
	0,\
	0);\

	beginRendering(context);
	DirectX::XMFLOAT4X4 matrixStore;
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_WORLD];
	renderTexture->clear(context, 0, 0, 0, 1);
	depthTexture->clear(context);
	//render the front face first
	renderTexture->setRenderTarget(context, depthTexture->getDepthStencilView());
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(viewMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matrixStore);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matrixStore);
	//shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);
	shaderVert.SetShader();
	shaderFrag.SetShader();
	RENDER_LIGHTS;
	if (renderTexture2 != 0 && depthTexture2 != 0) {
		renderTexture2->clear(context, 0, 0, 0, 1);
		depthTexture2->clear(context);
		renderTexture2->setRenderTarget(context, depthTexture2->getDepthStencilView());
		context->RSSetState(asset.RASTR_STATE_CULL_FRONT);
		RENDER_LIGHTS;
	}
	endRendering(context);
}

void NGraphic::GraphicMain::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
	Asset& asset, NScene::Scene& scene)
{
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		if(m_lightInfos.find(it->get()->m_id) != m_lightInfos.end()) continue;
		m_lightInfos[it->get()->m_id] = getLightInfo(device);
	}
	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	auto worldMatrixFrustum = DirectX::SimpleMath::Matrix::CreateRotationX(3.14 / 2);
	auto viewMatirx = scene.m_camMain.getViewMatrix();
	auto projMatrix = scene.m_camMain.getProjectionMatrix(m_width, m_height);
	renderWorldNormalDiffuse(device, context, asset, scene, worldMatrix);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		LightInfo& lightInfo = m_lightInfos[it->get()->m_id];
		//renderWorld(device, context, asset, scene, worldMatrix, lightInfo.position, lightInfo.depth);
		renderWorld(device, context, asset, scene,0,
			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix(lightInfo.position->getWidth(), lightInfo.position->getHeight()),
			lightInfo.position, lightInfo.depth);
		auto lightWorldMatirx = DirectX::XMMatrixMultiply(worldMatrixFrustum, it->get()->getModelMatrix());
		renderFrustum(device, context, asset, 
			lightWorldMatirx, viewMatirx, projMatrix,
			m_renderTextures[TARGET_LIGHTSHAFT_FRONT], m_depthTextures[TARGET_LIGHTSHAFT_FRONT],
			m_renderTextures[TARGET_LIGHTSHAFT_BACK], m_depthTextures[TARGET_LIGHTSHAFT_BACK]);
	}

	float colorClear[4] = {1,0,0,0};
	beginRendering(context);
	context->OMSetRenderTargets(1, &target, targetDepth);
	context->RSSetViewports(1, &viewport);
	context->ClearRenderTargetView(target, colorClear);
	context->ClearDepthStencilView(targetDepth, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	auto skyVS = asset.m_shadersVert[RENDER_SKYBOX].get();
	auto skyPS = asset.m_shadersFrag[RENDER_SKYBOX].get();
	
	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix( viewport.Width, viewport.Height))); // Transpose for HLSL!

	
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
