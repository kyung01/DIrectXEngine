#include <Engine\Engine.h>
#include <iostream>		
#include <DirectX\DirectXUtility.h>
#include <ScreenGrab.h>


#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;
const int CLUSTER_ITEM_SIZE = 3200;
const int CLUSTER_SIZE = 1000;
const float LIGHT_INFLUENCE_PER_INTENSITY = 12.0f;

//What is the size of the atals shadow map?
const int ATLAS_SHADOW_MAP_WIDTH = 5000;
const int ATLAS_SHADOW_MAP_HEIGHT = 5000;
//How much do I slice per light from the atlas map?
const int ATLAS_SHADOW_MAP_SLICE_SPOTLIGHT_WIDTH = 512*1;
const int ATLAS_SHADOW_MAP_SLICE_POINTLIGHT_WIDTH = 512*6;
const int ATLAS_SHADOW_MAP_SLICE_HEIGHT = 512*1;

void Engine::initExample()
{
	int ENTITY_NUMBER = 64;
	int RANDOM_MODEL_NUMBER = 5;
	int RANDOM_LIGHT_NUMBER = 5;
	int x(0), y(0), z(3);
	
	for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = m_entityFactory.addEntity();
		int entityIndex = m_entityFactory.m_entities.size() - 1;
		m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
		m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
		m_renderSystem.getLastComponent().meshId = MESH_SPHERE;
		switch (i%9) {
		default:
		case 0:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_GOLD_SCUFFED_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_GOLD_SCUFFED_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_GOLD_SCUFFED_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_GOLD_SCUFFED_METALNESS;
			break;
		case 1:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_METAL_ALBD;
			m_renderSystem.getLastComponent().roughMap = TXTURE_METAL_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_METAL_METALNESS;
			break;
		case 2:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_METAL_RUSTED_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_METAL_RUSTED_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_METAL_RUSTED_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_METAL_RUSTED_METALNESS;
			m_renderSystem.getLastComponent().aoMap = TXTURE_METAL_RUSTED_AO;
			break;
		case 3:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_METAL_GREASY_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_METAL_GREASY_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_METAL_GREASY_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_METAL_GREASY_METALNESS;
			break;
		case 4:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_COPPER_ROCK_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_COPPER_ROCK_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_COPPER_ROCK_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_COPPER_ROCK_METALNESS;
			m_renderSystem.getLastComponent().aoMap = TXTURE_COPPER_ROCK_AO;
			break;
		case 5:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_ROCK_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_ROCK_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_ROCK_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_ROCK_METALNESS;
			m_renderSystem.getLastComponent().aoMap = TXTURE_ROCK_AO;
			break;
		case 6:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_MARBEL_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_MARBEL_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_MARBEL_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_MARBEL_METALNESS;
			break;
		case 7:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_MAHOGFLOOR_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_MAHOGFLOOR_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_MAHOGFLOOR_ROUGH;
			m_renderSystem.getLastComponent().aoMap = TXTURE_MAHOGFLOOR_AO;
			break;
		case 8:
			m_renderSystem.getLastComponent().albedoMap = TXTURE_HARSHBRICKS_ALBD;
			m_renderSystem.getLastComponent().normalMap = TXTURE_HARSHBRICKS_NORMAL;
			m_renderSystem.getLastComponent().roughMap = TXTURE_HARSHBRICKS_ROUGH;
			m_renderSystem.getLastComponent().metalMap = TXTURE_HARSHBRICKS_METALNESS;
			m_renderSystem.getLastComponent().aoMap = TXTURE_HARSHBRICKS_AO;
			break;
		}


		m_transform3DSystem.getLastComponent().setPosition(-4 + x++, -4 + y, z);
		m_transform3DSystem.getLastComponent().setRotation(Quaternion::CreateFromAxisAngle(Vector3(rand(), rand(), rand()), rand()));
		if (x > 8) {
			x = 0;
			y++;
		}
	}
	for (int i = 0; i < RANDOM_LIGHT_NUMBER; i++) {
		int selectedEntityIndex;
		bool isNewEntityAvailable = false;
		int maxTolerableFailure = 100;
		do {
			selectedEntityIndex = rand() % m_entityFactory.m_entities.size();
			isNewEntityAvailable = (m_entityFactory.getEntity(selectedEntityIndex).m_lightComponent == 0);
		} 
		while (!isNewEntityAvailable && maxTolerableFailure-- > 0 );
		if (!isNewEntityAvailable) 
			continue;
		//std::cout << "SELECTED LIGHT INDEX " << selectedEntityIndex << std::endl;
		m_lightSystem.addEntity(m_entityFactory.m_entities, m_entityFactory.getEntity(selectedEntityIndex), selectedEntityIndex);
		m_lightSystem.getLastComponent().color = Vector3(1, 1, 1);
		if(m_lightSystem.getLastComponent().lightType == LIGHT_TYPE::POINT_LIGHT)
			m_renderSystem.getComponent(selectedEntityIndex).meshId = MESH_SPHERE;
		else
			m_renderSystem.getComponent(selectedEntityIndex).meshId = MESH_SPOTLIGHT;
		m_atlasSystem.addEntity(m_entityFactory.m_entities, m_entityFactory.getEntity(selectedEntityIndex), selectedEntityIndex);
		m_entityFactory.getEntity(selectedEntityIndex).m_transform3D->
			setPosition(
				m_entityFactory.getEntity(selectedEntityIndex).m_transform3D->position.x,
				m_entityFactory.getEntity(selectedEntityIndex).m_transform3D->position.y,
				-4);
		m_entityFactory.getEntity(selectedEntityIndex).m_transform3D->
			setRotation(Quaternion::Identity);
		//std::cout << "ATLAS " << m_atlasSystem.getLastComponent().x << " , " << m_atlasSystem.getLastComponent().y << " (" << m_atlasSystem.getLastComponent().width << "," << m_atlasSystem.getLastComponent().height << ")" << std::endl;

	} 
	{

		Vector3 positions[4] = { Vector3(0, 0, -5) ,Vector3(0, 0, +5) ,Vector3(-5, 0, 0),Vector3(+5, 0, 0) };
		float rotations[4] = { 0,3.14f,3.14f / 2.0f,-3.14f / 2.0f };
		for (int i = 0; i < 4; i++)
		{
			Entity& entity = m_entityFactory.addEntity();
			int entityIndex = m_entityFactory.m_entities.size() - 1;
			m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.getLastComponent().meshId = MESH_PLANE;
			m_transform3DSystem.getLastComponent().setPosition(positions[i]);
			m_transform3DSystem.getLastComponent().setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), rotations[i]));
			m_transform3DSystem.getLastComponent().setScale(Vector3(10, 10, 10));
			
		}
	}
	{

		Vector3 positions[2] = { Vector3(0, -5, 0) , Vector3(0, 5, 0)};
		float rotations[2] = {-3.14f/2, 3.14f / 2};
		for (int i = 0; i < 1; i++)
		{
			Entity& entity = m_entityFactory.addEntity();
			int entityIndex = m_entityFactory.m_entities.size() - 1;
			m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
			m_renderSystem.getLastComponent().meshId = MESH_PLANE;
			m_transform3DSystem.getLastComponent().setPosition(positions[i]);
			m_transform3DSystem.getLastComponent().setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), rotations[i]));
			m_transform3DSystem.getLastComponent().setScale(Vector3(10, 10, 10));
		}
	}
	m_physicsMain.init();
}

KEngine::Engine::Engine() :
	m_dataTranslator(CLUSTER_SIZE, CLUSTER_ITEM_SIZE, 256, 256, 256)
{
	m_eventHandlers.push_back(&m_handlerKeyboardInput);
}

void KEngine::Engine::init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight)
{
	print("init");
	m_asset.init(device, context);
	m_lightSystem.init(windowWidth/windowHeight, 0.01f, 100.0f, 10, 10, 10);
	m_atlasSystem.init(
		ATLAS_SHADOW_MAP_WIDTH, ATLAS_SHADOW_MAP_HEIGHT, 
		ATLAS_SHADOW_MAP_SLICE_SPOTLIGHT_WIDTH, ATLAS_SHADOW_MAP_SLICE_HEIGHT,
		ATLAS_SHADOW_MAP_SLICE_POINTLIGHT_WIDTH, ATLAS_SHADOW_MAP_SLICE_HEIGHT);
	m_frustum.init(windowWidth / (float)windowHeight, 0.1f, 100.0f, 10, 10, 10);
	initExample();
	m_textureAtlasShadowMap.init(device, ATLAS_SHADOW_MAP_WIDTH, ATLAS_SHADOW_MAP_HEIGHT);
	m_textureAtlasShadowMapDepth.init(device, ATLAS_SHADOW_MAP_WIDTH, ATLAS_SHADOW_MAP_HEIGHT);
	//initTest(device, context, windowWidth, windowHeight);


}
void Engine::initTest(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight) {

	HRESULT result;
	ID3D11Resource *cubemapResource;
	ID3D11Texture2D *pTextureInterface = 0;
	D3D11_TEXTURE2D_DESC descCubemap, desc;
	ID3D11Texture2D * textureStagingResource;

	// Initialize the render target texture description.
	ZeroMemory(&descCubemap, sizeof(descCubemap));
	ZeroMemory(&desc, sizeof(desc));

	m_asset.getCubeMap(CUBEMAP_SKYBOX_SUNNY)->GetResource(&cubemapResource);
	cubemapResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	pTextureInterface->GetDesc(&descCubemap);
	std::cout << "MIP MAP " << descCubemap.MipLevels << std::endl;
	desc = descCubemap;
	//turn the cubemap descrip into stating format
	//desc.Width = descCubemap.Width;
	//desc.Height = descCubemap.Height;
	//desc.MipLevels = 1;
	desc.ArraySize = 6;
	//desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0; //staing texture cannot be bound
	desc.MiscFlags =  D3D11_RESOURCE_MISC_TEXTURECUBE;
	//desc.SampleDesc.Count = 1;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	DirectX::DirectXUtility::HRESULT_CHECK(device->CreateTexture2D(&desc, NULL, &textureStagingResource));
	
	HRESULT h;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//don't copy resources for now
	
	context->CopyResource(textureStagingResource, cubemapResource);
	for (int faceindex = 3; faceindex < 4; faceindex++) {
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		DirectX::DirectXUtility::HRESULT_CHECK(context->Map(textureStagingResource, faceindex, D3D11_MAP_READ_WRITE, 0, &mappedResource));
		std::cout <<"FACE "<< faceindex <<" : "<<mappedResource.RowPitch << " , " << mappedResource.DepthPitch << std::endl;

		//DirectX::D3DX11SaveTextureToFile
		//memcpy((int8_t *)mappedResource.pData, pixels, sizeof(int8_t) *desc.Width*desc.Height * 4 * 6);
		int8_t* pointer = (int8_t*)mappedResource.pData;
		int8_t* pixels = new int8_t[1024 * 1024*4];

		for (int i = 0; i < 1024 * 1024; i++) {
			pixels[i * 4 + 0] = pointer[4 * (1024 * 1024) + 0];
			pixels[i * 4 + 1] = pointer[4 * (1024 * 1024) + 1];
			pixels[i * 4 + 2] = pointer[4 * (1024 * 1024) + 2];
			pixels[i * 4 + 3] = pointer[4 * (1024 * 1024) + 3];
		}
		for (int i = 0; i < 1024 * 1024; i++) {
			//pointer[i * 4 + 0] = pixels[i * 4 + 0];
			//pointer[i * 4 + 1] = pixels[i * 4 + 1];
			//pointer[i * 4 + 2] = pixels[i * 4 + 2];
			//pointer[i * 4 + 3] = pixels[i * 4 + 3];
			pointer[i * 4 + 0] = 255*(i%1024 / 1024.0f);
			pointer[i * 4 + 1] = pointer[i * 4 + 1]+255;
			pointer[i * 4 + 2] = pointer[i * 4 + 2]+255;
			pointer[i * 4 + 3] = pointer[i * 4 + 3];
		}
		for (int i = 0; i <mappedResource.DepthPitch*mappedResource.RowPitch; i++) {
			//pixels[i + 0] = pointer[i + 0];//= 0;// pixels[i + 0];
										   //pixels[i + 1] = pointer[i + 1];//= 0;// pixels[i + 1];
										   //pixels[i + 2] = pointer[i + 2];//= 0;// pixels[i + 2];
										   //pixels[i + 3] = pointer[i + 3];//=0;// pixels[i + 3];
										   //pixels[i] =255;
										   //pixels[i+1] = 0;
										   //pixels[i+2] = 0;
										   //pixels[i+3] = 255;
		}
		bool run = true;
		int index = 0;
	
		for (int i = 0; i <mappedResource.DepthPitch*mappedResource.RowPitch; i++) {
			//pointer[i + 0] = 0;
			//pointer[i + 0] = pixels[i + 0];//= 0;// pixels[i + 0];
										   //pointer[i + 1] = pixels[i + 1];//= 0;// pixels[i + 1];
										   //pointer[i + 2] = pixels[i + 2];//= 0;// pixels[i + 2];
										   //pointer[i + 3] = pixels[i + 3];//=0;// pixels[i + 3];
										   //pixels[i] =255;
										   //pixels[i+1] = 0;
										   //pixels[i+2] = 0;
										   //pixels[i+3] = 255;
		}


		//memcpy((int8_t *)mappedResource.pData, pixels, sizeof(int8_t) *desc.Width*desc.Height *desc.ArraySize * 4 );
		context->Unmap(textureStagingResource, faceindex);
		DirectX::SaveDDSTextureToFile(context, textureStagingResource, L"test.dds");
		context->CopyResource(cubemapResource, textureStagingResource);
		
		switch (faceindex)
		{
		default:
			DirectX::SaveDDSTextureToFile(context, textureStagingResource, L"1.dds");
			break;
		case 1:
			DirectX::SaveDDSTextureToFile(context, textureStagingResource, L"2.dds");
			break;
		case 2:
			DirectX::SaveDDSTextureToFile(context, textureStagingResource, L"3.dds");
			break;
		case 3:
			DirectX::SaveDDSTextureToFile(context, textureStagingResource, L"4.dds");
			break;
		}
		delete pixels;
	}
	//delete pixels;
	for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
		for (float j = 0; j < desc.Width; j++)
			for (float i = 0; i < desc.Height; i++)
			{
				

			}
	}
}
void Engine::update(float timeElapsed){
	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->update(timeElapsed);
	}
	m_renderSystem.setCameraPosition(m_handlerKeyboardInput.getPosition());
	m_renderSystem.setCameraRotation(m_handlerKeyboardInput.getRotation());
	m_renderSystem		.update(m_entityFactory.m_entities, timeElapsed);
	m_lightSystem		.update(m_entityFactory.m_entities, timeElapsed);
	m_transform3DSystem	.update(m_entityFactory.m_entities, timeElapsed);

	auto camViewMatrix = m_renderSystem.getCameraViewMatrix();


	m_frustum.testBegin();	
	for (int i = 0; i < m_lightSystem.getLightCount(); i++) {
		Vector3 lightPos, lightDirLook;
		float lightIntensity, lightFOV;
		//From Camera's perspective
		Vector3 pos, posDirLook, dir;
		LIGHT_TYPE lightType = m_lightSystem.getComponent(i).lightType;

		switch (lightType) {
		case LIGHT_TYPE::POINT_LIGHT:
			lightPos = m_lightSystem.getComponent(i).position;
			lightIntensity = m_lightSystem.getComponent(i).intensity;
			break;
		case LIGHT_TYPE::SPOT_LIGHT:
			lightPos = m_lightSystem.getComponent(i).position;
			lightDirLook = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_lightSystem.getComponent(i).rotation);

			lightIntensity = m_lightSystem.getComponent(i).intensity;
			lightFOV = m_lightSystem.getComponent(i).fov;
			break;
		}

		pos = DirectX::XMVector3Transform(lightPos, camViewMatrix);
		posDirLook = XMVector3Transform(lightPos + lightDirLook, camViewMatrix);
		dir = posDirLook - pos;

		
		if (lightType == LIGHT_TYPE::POINT_LIGHT) {
			m_frustum.testPointlight(i, pos,lightIntensity * LIGHT_INFLUENCE_PER_INTENSITY);
		}
		else if (lightType == LIGHT_TYPE::SPOT_LIGHT) {
			m_frustum.testSpotlight(i, pos, dir, lightIntensity* LIGHT_INFLUENCE_PER_INTENSITY, lightFOV);
		}
	}
	
	for (int i = 0; i < m_lightSystem.getLightCount(); i++) {
		KFrustum::NBuffer::LightParameter lightParameter = {};
		LightComponent& light = m_lightSystem.getComponent(i);
		AtlasComponent & atlasCompo = *m_entityFactory.getEntity(light.entityIndex).m_atlasComponent;
		lightParameter.isSpotlight = light.lightType == LIGHT_TYPE::SPOT_LIGHT;

		


		lightParameter.position = m_lightSystem.getComponent(i).position;
		lightParameter.color = m_lightSystem.getComponent(i).color;

		lightParameter.topLeftX = atlasCompo.x;
		lightParameter.topLeftY = atlasCompo.y;
		lightParameter.viewPortWidth = atlasCompo.width;
		lightParameter.viewPortHeight = atlasCompo.height;

		if (light.lightType == LIGHT_TYPE::SPOT_LIGHT) {
			//spot light
			lightParameter.angle = m_lightSystem.getComponent(i).fov;
			lightParameter.axis = (Vector3)DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_lightSystem.getComponent(i).rotation);

			Vector3 lookDir = DirectX::XMVector3Rotate(Vector3(0, 0, 1), light.rotation);
			DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookToLH(light.position, lookDir, Vector3::Up);
			DirectX::SimpleMath::Matrix projMatrix = DirectX::XMMatrixPerspectiveFovLH(
				light.fov,		// Field of View Angle
				atlasCompo.width / atlasCompo.height,		// Aspect ratio
				0.01f,100.0f);					// Far clip plane distance

			//DirectX::SimpleMath::Matrix projMatrix = DirectX::XMMatrixPerspectiveLH(atlasCompo.width, atlasCompo.height,0.1, 100);
			auto matViewProj = DirectX::XMMatrixMultiply(viewMatrix, projMatrix);
			DirectX::XMStoreFloat4x4(&lightParameter.matLight, XMMatrixTranspose(matViewProj));

		}
		if (light.lightType == LIGHT_TYPE::POINT_LIGHT) {
			DirectX::SimpleMath::Matrix projMatrix = DirectX::XMMatrixPerspectiveLH(atlasCompo.width / 6.0f, atlasCompo.height, 0.1, 100);
			DirectX::XMStoreFloat4x4(&lightParameter.matLight, XMMatrixTranspose(projMatrix));
		}
		m_dataTranslator.translateLight(lightParameter, i);
		//std::cout << "DateTranslator LightColor : " << lightParameter.color.x << " " << lightParameter.color.y << " " << lightParameter.color.z << std::endl;
	}
	m_dataTranslator.translate(m_frustum.m_clusters);
}

void Engine::renderUpdate(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
}

int renderAtalsOnce = 100;
Renderable renderable_cubemap;
void Engine::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{

	if (renderAtalsOnce++ > 0)
	{
		//Prepare rendering the atlas
		D3D11_VIEWPORT atlasViewport = {};
		auto &vertShader = m_asset.getVertShader(RENDER_WORLD_POSITION);
		auto &fragShader = m_asset.getFragShader(RENDER_WORLD_POSITION);
		atlasViewport.MinDepth = 0;
		atlasViewport.MaxDepth = 1;

		float colorClean[4] = { 0.1f,0.1f,0.1f,1 };
		context->ClearRenderTargetView(m_textureAtlasShadowMap.getRenderTargetView(), colorClean);
		context->ClearDepthStencilView(m_textureAtlasShadowMapDepth.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		for (int i = 0; i < m_lightSystem.getComponentVectorSize(); i++) {
			LightComponent& lightComponent = m_lightSystem.getComponent(i);
			Transform3D&	transform = m_transform3DSystem.getComponent(lightComponent.entityIndex);
			AtlasComponent& atalsComponent = *m_entityFactory.getEntity(lightComponent.entityIndex).m_atlasComponent;
			atlasViewport.TopLeftX = atalsComponent.x;
			atlasViewport.TopLeftY = atalsComponent.y;
			atlasViewport.Width = atalsComponent.width;
			atlasViewport.Height = atalsComponent.height;
			if (lightComponent.lightType == LIGHT_TYPE::POINT_LIGHT) {

				m_renderSystem.renderPointLightShadowMap(
					device, context,
					transform.position,
					m_textureAtlasShadowMap.getRenderTargetView(), m_textureAtlasShadowMapDepth.getDepthStencilView(), atlasViewport,
					m_asset.getRasterizer(KEnum::RASTR_CULL_BACK), vertShader, fragShader, m_asset.m_meshes, m_entityFactory);

			}
			else {
				m_renderSystem.renderSpotLightShadowMap(
					device, context,
					transform.position, transform.rotation, lightComponent.fov,
					m_textureAtlasShadowMap.getRenderTargetView(), m_textureAtlasShadowMapDepth.getDepthStencilView(), atlasViewport,
					m_asset.getRasterizer(KEnum::RASTR_CULL_BACK), vertShader, fragShader, m_asset.m_meshes, m_entityFactory);

			}
		}
		renderAtalsOnce = -100;
	}


	//Render the scene
	context->OMSetRenderTargets(1, &target, targetDepth);
	context->RSSetViewports(1, &viewport);
	{
		//LIGHT_ATLAS_WIDTH
		//m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetShaderResourceView("textureProbe", textureProbe);
	}
	m_dataTranslator.transfer(
		context,
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).GetBuffer(0), m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).GetBuffer(1),
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).GetBuffer(2), 0, 0);
	Matrix matView = m_renderSystem.getCameraViewMatrix();
	{
		DirectX::XMFLOAT4X4 MAT_TEMP;
		DirectX::XMStoreFloat4x4(&MAT_TEMP, XMMatrixTranspose(matView));
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetShaderResourceView("textureLightAtlas", m_textureAtlasShadowMapDepth.getShaderResourceView());
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetInt("LIGHT_ATLAS_WIDTH", ATLAS_SHADOW_MAP_WIDTH);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetInt("LIGHT_ATLAS_HEIGHT", ATLAS_SHADOW_MAP_HEIGHT);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetMatrix4x4("eyeViewMatrix", MAT_TEMP);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetInt("frustumX", m_frustum.m_size.x);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetInt("frustumY", m_frustum.m_size.y);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetInt("frustumZ", m_frustum.m_size.z);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetFloat("frustumSizeRatio", m_frustum.m_widthOverHeight);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetFloat("frustumNear", m_frustum.m_near);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetFloat("frustumFar", m_frustum.m_far);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).CopyAllBufferData();

		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("samplerDefault", m_asset.m_sampler);

		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("AlbedoSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("NormalSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("RoughSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("MetalSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("HeightSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetSamplerState("AOSampler", m_asset.m_sampler);
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetFloat3("EYE_POS", m_renderSystem.getCameraPosition());
	}

	m_renderSystem.render(
		device, context, target, targetDepth, viewport,
		m_asset,
		m_asset.getRasterizer(KEnum::RASTR_CULL_BACK),
		m_asset.getVertShader(KEnum::RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM), m_asset.getFragShader(KEnum::RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM),
		m_asset.m_meshes,m_entityFactory);
	{
		//LIGHT_ATLAS_WIDTH
		m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetShaderResourceView("textureLightAtlas", 0);
		//m_asset.getFragShader(RENDER_FORWARD_ATLAS_CLUSTERED_FRUSTUM).SetShaderResourceView("textureProbe", textureProbe);
	}
	//m_renderSystem.renderCubemap(device, context, target, targetDepth, viewport, m_asset, renderable_cubemap);



}
void Engine::OnMouseMove(WPARAM buttonState, int x, int y)
{
	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->OnMouseMove(buttonState, x, y);
	}

	if (!(buttonState & 0x0001)) //if button is not pressed
	{
		return;
	}
}

void KEngine::Engine::OnResize(int windowWidth, int windowHeight)
{
	m_renderSystem.OnResize(windowWidth, windowHeight);
}
