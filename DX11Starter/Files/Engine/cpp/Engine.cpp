#include <Engine\Engine.h>
#include <iostream>		
#include <DirectX\DirectXUtility.h>

#define print(content) std::cout<<"Engine : "<< content << "\n";
using namespace KEngine;
const int CLUSTER_ITEM_SIZE = 3200;
const int CLUSTER_SIZE = 1000;
const float LIGHT_INFLUENCE_PER_INTENSITY = 12.0f;
void Engine::initExample()
{
	int ENTITY_NUMBER = 50;
	int RANDOM_MODEL_NUMBER = 5;
	int RANDOM_LIGHT_NUMBER = 10;
	int x(0), y(0), z(3);
	for (int i = 0; i < ENTITY_NUMBER; i++)
	{
		Entity& entity = m_entityFactory.addEntity();
		int entityIndex = m_entityFactory.m_entities.size() - 1;
		m_transform3DSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
		m_renderSystem.addEntity(m_entityFactory.m_entities, entity, entityIndex);
		m_renderSystem.getLastComponent().meshId = getRadnomModelID();

		m_transform3DSystem.getLastComponent().setPosition(-4+x++, y, z);
		if (x > 8) {
			x = 0;
			y++;
		}
		//std::cout << i << " POISTION " << m_transform3DSystem.getComponent(i).position.x << " " << m_transform3DSystem.getComponent(i).position.y << std::endl;
		//std::cout << i << " MESH ID " << m_renderSystem.getComponent(i).meshId << std::endl;
		//std::cout << i << " POISTION " << m_renderSystem.getLastComponent().getPosition().x << " " << m_renderSystem.getLastComponent().getPosition().y << std::endl;
		//std::cout << i << " MESH ID " << m_renderSystem.getLastComponent().meshId << std::endl;
		//m_renderSystem.getLastComponent().setPosition(Vector3(i, j, 1));
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
		std::cout << "SELECTED LIGHT INDEX " << selectedEntityIndex << std::endl;
		m_lightSystem.addEntity(m_entityFactory.m_entities, m_entityFactory.getEntity(selectedEntityIndex), selectedEntityIndex);
		if(m_lightSystem.getLastComponent().lightType == LIGHT_TYPE::POINT_LIGHT)
			m_renderSystem.getComponent(selectedEntityIndex).meshId = MESH_SPHERE;
		else
			m_renderSystem.getComponent(selectedEntityIndex).meshId = MESH_SPOTLIGHT;
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

		Vector3 positions[2] = { Vector3(0, 5, 0) ,Vector3(0, -5, 0) };
		float rotations[2] = { 3.14f/2,-3.14f/2};
		for (int i = 0; i < 4; i++)
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
	m_frustum.init(windowWidth / (float)windowHeight, 0.1f, 100.0f, 10, 10, 10);
	initExample();
}
void Engine::update(float timeElapsed)
{
	for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++) {
		(*it)->update(timeElapsed);
	}
	m_renderSystem.setCameraPosition(m_handlerKeyboardInput.getPosition());
	m_renderSystem.setCameraRotation(m_handlerKeyboardInput.getRotation());
	m_renderSystem		.update(m_entityFactory.m_entities, timeElapsed);
	m_transform3DSystem	.update(m_entityFactory.m_entities, timeElapsed);
	m_lightSystem		.update(m_entityFactory.m_entities, timeElapsed);


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
		LIGHT_TYPE lightType = m_lightSystem.getComponent(i).lightType;
		lightParameter.isSpotlight = lightType == LIGHT_TYPE::SPOT_LIGHT;
		//std::cout << "LIGHT TYPE is spotLight " << ((lightParameter.isSpotlight)?"SPOTLIGHT":"POINTLIGHT") << std::endl;
		//system("pause");
		if (lightType == LIGHT_TYPE::POINT_LIGHT) {
			lightParameter.position = m_lightSystem.getComponent(i).position;
			lightParameter.color = m_lightSystem.getComponent(i).color;
		}
		else {
			//spot light
			lightParameter.position = m_lightSystem.getComponent(i).position;
			lightParameter.color = m_lightSystem.getComponent(i).color;
			lightParameter.angle = m_lightSystem.getComponent(i).fov;
			lightParameter.axis = (Vector3)DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_lightSystem.getComponent(i).rotation);
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

void Engine::render(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport)
{
	m_dataTranslator.transfer(
		context,
		m_asset.getFragShader(RENDER_TEST).GetBuffer(0), m_asset.getFragShader(RENDER_TEST).GetBuffer(1),
		m_asset.getFragShader(RENDER_TEST).GetBuffer(2), 0, 0);
	Matrix matView = m_renderSystem.getCameraViewMatrix();
	{
		DirectX::XMFLOAT4X4 MAT_TEMP;
		DirectX::XMStoreFloat4x4(&MAT_TEMP, XMMatrixTranspose(matView));
		m_asset.getFragShader(RENDER_TEST).SetMatrix4x4("eyeViewMatrix", MAT_TEMP);
		m_asset.getFragShader(RENDER_TEST).SetInt("frustumX", m_frustum.m_size.x);
		m_asset.getFragShader(RENDER_TEST).SetInt("frustumY", m_frustum.m_size.y);
		m_asset.getFragShader(RENDER_TEST).SetInt("frustumZ", m_frustum.m_size.z);
		m_asset.getFragShader(RENDER_TEST).SetFloat("frustumSizeRatio", m_frustum.m_widthOverHeight);
		m_asset.getFragShader(RENDER_TEST).SetFloat("frustumNear", m_frustum.m_near);
		m_asset.getFragShader(RENDER_TEST).SetFloat("frustumFar", m_frustum.m_far);
		m_asset.getFragShader(RENDER_TEST).CopyAllBufferData();
		m_asset.getFragShader(RENDER_TEST).SetSamplerState("samplerDefault",m_asset.m_sampler);
	}

	m_renderSystem.render(
		device, context, target, targetDepth, viewport,
		m_asset.getRasterizer(KEnum::RASTR_CULLBACKFACE),
		m_asset.getVertShader(KEnum::RENDER_TEST), m_asset.getFragShader(KEnum::RENDER_TEST),
		m_asset.m_meshes,m_entityFactory);
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
