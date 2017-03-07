#include "Graphic\GraphicMain.h"
#include <string>
#include "Graphic\RenderInstruction.h"
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
	int TEXTURE_LIGHT_ATLAS_UNIT = 64;
	int TEXTURE_LIGHT_ATLAS_SIZE = 20;

#define INIT_DEPTH_TEXTURE(key,defWidth, defHeight) \
m_depthTextures[key] = std::shared_ptr<DepthTexture>(new DepthTexture());\
m_depthTextures[key]->init(device, defWidth, defHeight);
#define INIT_RENDER_TEXTURE(key,defWidth, defHeight) \
this->m_renderTextures[key]	= std::shared_ptr<RenderTexture>(new RenderTexture());\
this->m_renderTextures[key]	->init(device, defWidth, defHeight);
	std::cout << "initTextures\n";
	
	INIT_RENDER_TEXTURE(TARGET_WORLD,			width, height);
	INIT_RENDER_TEXTURE(TARGET_NORMAL,			width, height);
	INIT_RENDER_TEXTURE(TARGET_DIFFUSE,			width, height);
	INIT_RENDER_TEXTURE(TARGET_PROPERTY,		width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_FRONT,width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHTSHAFT_BACK, width, height);
	INIT_RENDER_TEXTURE(TARGET_FINAL, width, height);
	INIT_RENDER_TEXTURE(TARGET_TEST, width, height);
	INIT_RENDER_TEXTURE(TARGET_LIGHT_ATLAS, TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE, TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE);

	INIT_DEPTH_TEXTURE(DEPTH_WORLD, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_FINAL, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_LIGHTSHAFT_FRONT, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_LIGHTSHAFT_BACK, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_TEST, width, height);
	INIT_DEPTH_TEXTURE(DEPTH_LIGHT_ATLAS, TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE, TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE);
	m_atlasSlicer = std::make_shared<TextureAtlasSlicer>(
		TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE, TEXTURE_LIGHT_ATLAS_UNIT*TEXTURE_LIGHT_ATLAS_SIZE,
		TEXTURE_LIGHT_ATLAS_SIZE, TEXTURE_LIGHT_ATLAS_SIZE);
	//m_depthTextures[TARGET_LIGHTSHAFT_BACK] = m_depthTextures[TARGET_LIGHTSHAFT_FRONT];

	return true;
}
void GraphicMain::updateBufferLightPrameter(
	ID3D11DeviceContext *context, ID3D11Buffer* buffer, std::list<std::shared_ptr<NScene::Light>>& lights)
{
	int index = 0;
	NBuffer::LightParameter parameter;
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;
		auto &info = m_lightInfos[it->get()->m_id];
		parameter.angle = light.getFOV();
		parameter.axis = light.m_dirLook;
		parameter.color = light.getLightColor();
		parameter.isSpotlight = light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT;
		parameter.position = light.m_pos;
		parameter.topLeftX = info.topLeftX;
		parameter.topLeftY = info.topLeftY;
		parameter.viewPortWidth = info.viewportWidth;
		parameter.viewPortHeight = info.viewportHeight;
		m_lightBuffer->setData(parameter, index++);

		//parameter.inverseViewProjX
	}
	m_lightBuffer->setData(context, buffer);
	
}

void GraphicMain::updateLightAtlas(std::list<std::shared_ptr<NScene::Light>> &lights)
{
	float size = 3;
	
	m_atlasSlicer->clear();
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;
		auto &info =  m_lightInfos[light.m_id];
		//0 spotlight
		//1 pointlight
		if (light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT) {

			if (!m_atlasSlicer->getRoom(info.topLeftX, info.topLeftY, info.viewportWidth, info.viewportHeight, (int)size, (int)size)) {
				std::cout << "GraphicMain::updateLightAtlas-> Updating Light Atals Failed.\n";
				system("pause");
			}
			else {
				size += 1.9f;
				//std::cout << "GraphicMain::updateLightAtlas-> Received available space\n";
				//std::cout << it->second.topLeftX << " , " << it->second.topLeftY<< " , " << it->second.viewportWidth << " , " << it->second.viewportHeight<<"\n";

				//success

			}
		}
		else {
			if (!m_atlasSlicer->getRoom(info.topLeftX, info.topLeftY, info.viewportWidth, info.viewportHeight, 3 * 6, 3)) {
				std::cout << "GraphicMain::updateLightAtlas-> Updating Light Atals Failed.\n";
				system("pause");
			}
			else {
				//std::cout << "GraphicMain::updateLightAtlas-> Received available space\n";
				//std::cout << it->second.topLeftX << " , " << it->second.topLeftY<< " , " << it->second.viewportWidth << " , " << it->second.viewportHeight<<"\n";

				//success

			}

		}
	}
}
void GraphicMain::renderLightAtlas(ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene)
{
	beginRendering(context);
	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	m_renderTextures[TARGET_LIGHT_ATLAS]->clear(context,0,0,0,1);
	//m_depthTextures[DEPTH_LIGHT_ATLAS]->clear(context);


	m_depthTextures[DEPTH_LIGHT_ATLAS]->clear(context);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		auto &light = **it;
		auto &lightInfo = m_lightInfos[light.m_id];
		//What kinds of lights are there ?
		//if (light.m_lightType != NScene::LIGHT_TYPE::SPOTLIGHT) continue;
		
		if(light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT)
			RenderInstruction::RENDER_LIGHT_ATLAS_SPOT(
			device, context, asset,
			*m_renderTextures[TARGET_LIGHT_ATLAS], *m_depthTextures[DEPTH_LIGHT_ATLAS],
			scene,

			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix(lightInfo.viewportWidth, lightInfo.viewportHeight ), 
			lightInfo.topLeftX, lightInfo.topLeftY, lightInfo.viewportWidth, lightInfo.viewportHeight);
		else {
			auto pointLight = static_cast<NScene::PointLight*>(&light);
			RenderInstruction::RENDER_LIGHT_ATLAS_POINT(
				device, context, asset,
				*m_renderTextures[TARGET_LIGHT_ATLAS], *m_depthTextures[DEPTH_LIGHT_ATLAS],
				scene,

				worldMatrix, 
				pointLight->getMatrixXPlus(), pointLight->getMatrixXMinus(),
				pointLight->getMatrixYPlus(), pointLight->getMatrixYMinus(), 
				pointLight->getMatrixZPlus(), pointLight->getMatrixZMinus(), 
				(**it).getProjectionMatrix(lightInfo.viewportWidth/6, lightInfo.viewportHeight),
				lightInfo.topLeftX, lightInfo.topLeftY, lightInfo.viewportWidth, lightInfo.viewportHeight);
		}

		
		//RenderInstruction::RENDER_LIGHT_ATLAS_SPOT(
		//	device, context, asset, scene,
		//	*m_renderTextures[TARGET_LIGHT_ATLAS], *m_depthTextures[DEPTH_LIGHT_ATLAS],
		//	light,
		//	m_lightInfos[light.m_id].topLeftX, m_lightInfos[light.m_id].topLeftY, m_lightInfos[light.m_id].viewportWidth, m_lightInfos[light.m_id].viewportHeight);
	}
	endRendering(context);
}

GraphicMain::GraphicMain()
{
}
bool GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, 
	int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight)
{
	float 
		NEAR_DISTANCE(1),
		FAR_DISTANCE(10),
		X_DIIVIDE(10),
		Y_DIVIDE(10),
		Z_DIVIDE(10),
		CLUSTER_ITEM_SIZE(X_DIIVIDE*Y_DIVIDE*Z_DIVIDE*10);
	this->m_width = width;
	this->m_height = height;
	m_rsm_flux_eye_perspective_width = textureIndirectLightWidth;
	m_rsm_flux_eye_perspective_height = textureIndirectLightHeight;
	m_frustum.init(3.14 / 2, NEAR_DISTANCE, FAR_DISTANCE, X_DIIVIDE, Y_DIVIDE, Z_DIVIDE);
	m_bufferDataTranslator = std::make_shared<BufferDataTranslator>(X_DIIVIDE* Y_DIVIDE* Z_DIVIDE, CLUSTER_ITEM_SIZE,256,256,256);
	m_lightBuffer = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::LightParameter>>(10);

	if (
		!initTextures(device,context,width,height, textureIndirectLightWidth, textureIndirectLightHeight)
		
		) return false;

	orthoView = DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up);
	orthoMVP = DirectX::XMMatrixMultiply( 
		DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up) , 
		DirectX::XMMatrixOrthographicLH(1,1,0.1,100) );

	return true;
}

void GraphicMain::update(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, NScene::Scene & scene)
{
	//m_bufferDataTranslator->constrcut();
	m_frustum.testBegin();
	int index = 0;
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++, index++) {
		auto &light = **it;
		Vector3 pos = XMVector3Transform(light.m_pos, scene.m_camMain.getViewMatrix());
		Vector3 posDirLook = XMVector3Transform(light.m_pos + light.m_dirLook, scene.m_camMain.getViewMatrix());
		Vector3 dir = posDirLook - pos;

		switch (light.m_lightType) {
		case NScene::POINTLIGHT:
			//m_frustum.testPointlight(index, light.m_pos, light.m_lightDistance);
			m_frustum.testPointlight(index, pos, light.m_lightDistance);
			break;
		case NScene::SPOTLIGHT:
			m_frustum.testSpotlight(index, pos, dir, light.m_lightDistance,light.getFOV());
			break;
		}
	}
}










void NGraphic::GraphicMain::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
	Asset& asset, NGame::Context &game	
	)
{
	NScene::Scene & scene = *game.m_scene;

	bool newLightInfo = false;
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		if (m_lightInfos.find(it->get()->m_id) != m_lightInfos.end()) continue;
		m_lightInfos[it->get()->m_id] = getLightInfo(device);
		newLightInfo = true;
	}
	if(newLightInfo)
		updateLightAtlas(scene.objs_lights);
	m_bufferDataTranslator->translate(m_frustum.m_clusters);
	m_bufferDataTranslator->translate(scene.objs_lights, m_lightInfos);
	m_bufferDataTranslator->transfer(
		asset.m_shadersFrag[RENDER_TEST]->GetBuffer(0), asset.m_shadersFrag[RENDER_TEST]->GetBuffer(1),
		asset.m_shadersFrag[RENDER_TEST]->GetBuffer(2), asset.m_shadersFrag[RENDER_TEST]->GetBuffer(3), asset.m_shadersFrag[RENDER_TEST]->GetBuffer(4));
	
	m_bufferDataTranslator->m_lights->setData(context, asset.m_shadersFrag[RENDER_TEST]->GetBuffer(0));

	//updateBufferLightPrameter(context, asset.m_shadersFrag[RENDER_TEST]->GetBuffer(0)   ,scene.objs_lights);

	m_renderTextureDummy.setRenderTargetView(target, viewport);
	m_depthTextureDummy.setDepthStencilView(targetDepth);
	m_renderTextureDummy.clear(context, 0, 0, 0, 0);
	m_depthTextureDummy.clear(context);
	DirectX::SimpleMath::Matrix matSceneMvpFirstPerson = 
		scene.m_camMain.getProjectionMatrix(viewport.Width, viewport.Height) *
		scene.m_camMain.getViewMatrix() * scene.m_camMain.getModelMatrix();
	
	if(true){
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

	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	auto worldMatrixFrustum = DirectX::SimpleMath::Matrix::CreateRotationX(3.14 / 2);
	auto viewMatirx = scene.m_camMain.getViewMatrix();
	auto projMatrix = scene.m_camMain.getProjectionMatrix(m_width, m_height);
	
	//context->OMSetBlendState(asset.BLEND_STATE_, 0, 0xffffffff);
	//context->RSSetState(asset.RASTR_STATE_CULL_BACK);
	RenderInstruction::RENDER_WORLD_NORMAL_DIFFUSE(
		device, context, asset, scene,
		*m_renderTextures[TARGET_WORLD],
		*m_renderTextures[TARGET_NORMAL],
		*m_renderTextures[TARGET_DIFFUSE],
		*m_renderTextures[TARGET_PROPERTY],
		*m_depthTextures[DEPTH_WORLD],
		worldMatrix);
	m_renderTextures[TARGET_TEST]->clear(context, 0, 0, 0, 0);
	m_depthTextures[DEPTH_TEST]->clear(context);
	RenderInstruction::RENDER_TEST(device, context, asset, scene, *m_renderTextures[TARGET_TEST], *m_depthTextures[DEPTH_TEST], worldMatrix, viewMatirx, projMatrix, 0);
	//now start rendering real stuff
	

	m_renderTextures[TARGET_FINAL]->clear(context, 0, 0, 0, 1);
	

	
	renderLightAtlas(device, context, asset, *game.m_scene);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
	

		NScene::Light& light = **it;
		LightInfo& lightInfo = m_lightInfos[it->get()->m_id];

		lightInfo.position->clear(context, 0, 0, 0, 1);
		lightInfo.depth->clear(context);

		//Here we are feeding light buffer information we need to render the scene
		RenderInstruction::RENDER_WORLD(
			device, context, asset,
			*lightInfo.position, *lightInfo.depth,
			scene,
			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix(lightInfo.position->getWidth(), lightInfo.position->getHeight())
			);
		auto lightWorldMatirx = DirectX::XMMatrixMultiply(worldMatrixFrustum, it->get()->getModelMatrix());


		DirectX::XMMATRIX lightMVP = DirectX::XMMatrixMultiply(light.getViewMatrix(), light.getProjectionMatrix(lightInfo.position->getWidth(), lightInfo.position->getHeight()));
		if (true) {
			m_depthTextureDummy.clear(context);
			m_depthTextures[DEPTH_FINAL]->clear(context);
			RenderInstruction::RENDER_DIRECT_LIGHT(
				device, context,asset,
				*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
				orthoMVP,
				//m_renderTextureDummy, m_depthTextureDummy,
				//m_renderTextureDummy, m_depthTextureDummy, 
				
				scene.m_camMain.m_pos,
				light.m_pos, light.m_dirLook, light.getLightColor(), light.getFOV() * RATIO_LIGHT_INNER, light.getFOV(),
				m_lightInfos[it->get()->m_id].position, lightMVP, light.getFOV(), 
				*m_renderTextures[TARGET_WORLD],
				*m_renderTextures[TARGET_NORMAL],
				*m_renderTextures[TARGET_DIFFUSE]
				);
		}

		if (true) {
			beginRendering(context);
			RenderInstruction::RENDER_FRUSTUM(
				device, context, asset,
				scene.m_camMain.m_pos,
				lightWorldMatirx, viewMatirx, projMatrix,
				*m_renderTextures[TARGET_LIGHTSHAFT_FRONT], *m_depthTextures[DEPTH_LIGHTSHAFT_FRONT],
				*m_renderTextures[TARGET_LIGHTSHAFT_BACK], *m_depthTextures[DEPTH_LIGHTSHAFT_BACK]
			);
			endRendering(context);

			beginRendering(context);
			RenderInstruction::RENDER_LIGHTSHAFT_SPOTLIGHT(
				device, context,
				asset,
				//m_renderTextureDummy, m_depthTextureDummy,
				*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
				orthoMVP,
				scene.m_camMain.m_pos, scene.m_camMain.m_dirLook,
				light.m_pos, light.m_dirLook, light.getLightColor(), light.getFOV() * RATIO_LIGHT_INNER, light.getFOV(),1.0f,
				*m_renderTextures[TARGET_WORLD],
				*m_renderTextures[TARGET_LIGHTSHAFT_FRONT],
				*m_renderTextures[TARGET_LIGHTSHAFT_BACK],
				*m_lightInfos[it->get()->m_id].position, lightMVP, light.getFOV());
			endRendering(context);
			
		}

		if (false) {
			m_depthTextureDummy.clear(context);
			//renderSkyboxReflection(device, context, m_renderTextureDummy, m_depthTextureDummy, asset,
			//	scene.m_camMain.m_pos,
			//
			//	asset.m_texturesCubeMap[TEXTURE_ID_SKYBOX_SUNNY], m_renderTextures[TARGET_WORLD]->getShaderResourceView(), m_renderTextures[TARGET_NORMAL]->getShaderResourceView());

		}

	}
	if (true) {
		m_depthTextures[DEPTH_FINAL]->clear(context);
		RenderInstruction::RENDER_DEBUG(
			device, context, asset,
			*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
			game, scene, m_frustum,
			*m_depthTextures[DEPTH_WORLD]);

	}

}

