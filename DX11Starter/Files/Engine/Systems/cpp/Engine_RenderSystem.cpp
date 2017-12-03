#include <Engine\Systems\RenderSystem.h>
#include <Engine\Componenets\LightEntity.h>
using namespace KEngine;
using namespace KEngine::KSystem;

void KEngine::KSystem::RenderSystem::addEntityHandle(Entity & entity, Renderable & componenet)
{
	//no handler codes required
}

void KEngine::KSystem::RenderSystem::addEntityLinkRecreate(std::vector<Entity> & entityVectors, Renderable & componenet)
{
	entityVectors[componenet.entityIndex].m_renderable = &componenet;
}

void RenderSystem::hprRenderMesh(ID3D11DeviceContext * context, Asset & asset, KEnum meshID)
{
	Mesh& mesh = asset.getMesh(meshID);
	stride = sizeof(Vertex);
	offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
	context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
}

void KEngine::KSystem::RenderSystem::init(int renderTargetWidth, int renderTargetHeight)
{
	m_camera.setProjParameters((3.14f / 2.0f), renderTargetWidth, renderTargetHeight, 0.1f, 1000.0f);
}

DirectX::SimpleMath::Matrix KEngine::KSystem::RenderSystem::getCameraViewMatrix()
{
	return m_camera.getViewMatrix();
}

Vector3 KEngine::KSystem::RenderSystem::getCameraPosition()
{
	return m_camera.getPosition();
}

void KEngine::KSystem::RenderSystem::setCameraProjectionFOV(float ratio)
{
}

void KEngine::KSystem::RenderSystem::setCameraPosition(Vector3 position)
{
	m_camera.setPosition(position);
}

void KEngine::KSystem::RenderSystem::setCameraRotation(Quaternion rotation)
{
	m_camera.setQuaternion(rotation);
}


void RenderSystem::setMatrix(ISimpleShader * shader, std::string name, DirectX::XMMATRIX matrix) {

	DirectX::XMStoreFloat4x4(&matTemp, XMMatrixTranspose(matrix));
	shader->SetMatrix4x4(name, matTemp);
}

void RenderSystem::setRenderTarget(
	ID3D11DeviceContext* deviceContext,
	ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);
}
void RenderSystem::renderMesh(ID3D11DeviceContext * context, Mesh & mesh, UINT offsetBegin, UINT stride)
{
	context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offsetBegin);
	context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
}
void KEngine::KSystem::RenderSystem::renderCubemap(
	ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport, 
	Asset & asset, Renderable& renderable)
{
	float colorClean[4] = { 0.1f,0.1f,0.1f,1 };
	auto &vertexShader = asset.getVertShader(RNDR_CUBEMAP);
	auto &fragmentShader = asset.getFragShader(RNDR_CUBEMAP);
	context->RSSetState(asset.getRasterizer(RASTR_CULL_BACK));
	setRenderTarget(context, renderTargetView, depthStencilView, viewport);
	vertexShader.SetShader();
	fragmentShader.SetShader();

	setMatrix(&vertexShader, "view", m_camera.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_camera.getProjMatrix());


	Mesh& mesh = asset.getMesh(MESH_SPHERE);
	setMatrix(&vertexShader, "world", renderable.getWorldMatrix());
	vertexShader.CopyAllBufferData();


	fragmentShader.SetShaderResourceView("Cubemap", asset.getCubeMap(CUBEMAP_SKYBOX_SUNNY));
	fragmentShader.SetSamplerState("Sampler", asset.m_sampler);

	fragmentShader.CopyAllBufferData();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
	context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
}
void RenderSystem::renderSkybox(ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport, Asset & asset, EntityFactory & entityFactory)
{
	context->RSSetState(asset.getRasterizer(RASTR_CULL_FRONT));

	auto& vertexShader		= asset.getVertShader(RNDR_SKY);
	auto& fragmentShader	= asset.getFragShader(RNDR_SKY);
	vertexShader.SetShader();
	fragmentShader.SetShader();

	setMatrix(&vertexShader, "view", m_camera.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_camera.getProjMatrix());
	setMatrix(&vertexShader, "world", Matrix::Identity);
	fragmentShader.SetShaderResourceView("Cubemap", asset.getCubeMap(CUBEMAP_SKYBOX_SUNNY));
	fragmentShader.SetSamplerState("Sampler", asset.m_sampler);
	vertexShader.CopyAllBufferData();
	fragmentShader.CopyAllBufferData();
	hprRenderMesh(context, asset, MESH_SPHERE);
}
void RenderSystem::render(
	ID3D11Device * device, ID3D11DeviceContext * context,
	ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
	KEngine::Asset &asset,
	ID3D11RasterizerState *cullBackFace,
	SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader,
	std::map<KEnum, Mesh> &meshes,
	KEngine::EntityFactory & entityFactory
)
{
	float colorClean[4] = { 0.1f,0.1f,0.1f,1 };
	setRenderTarget(context, renderTargetView, depthStencilView, viewport);
	context->ClearRenderTargetView(renderTargetView, colorClean);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//Render skybox
	renderSkybox(device, context, renderTargetView, depthStencilView, viewport, asset, entityFactory);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->RSSetState(cullBackFace);
	vertexShader.SetShader();
	fragmentShader.SetShader();
	
	setMatrix(&vertexShader, "view", m_camera.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_camera.getProjMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED) 
			continue;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent == 0) {
			fragmentShader.SetFloat3("diffuseColor", Vector3());

		}
		else {
			fragmentShader.SetFloat3("diffuseColor", lightComponent->color );

		}

		fragmentShader.SetShaderResourceView("AlbedoMap", asset.getTexture(it->albedoMap));
		fragmentShader.SetShaderResourceView("NormalMap", asset.getTexture(it->normalMap));
		fragmentShader.SetShaderResourceView("RoughMap", asset.getTexture(it->roughMap));
		fragmentShader.SetShaderResourceView("MetalMap", asset.getTexture(it->metalMap));
		fragmentShader.SetShaderResourceView("HeightMap", asset.getTexture(TXTURE_WHITE));
		fragmentShader.SetShaderResourceView("AOMap", asset.getTexture(it->aoMap));

		fragmentShader.CopyAllBufferData();
		hprRenderMesh(context, asset, it->meshId);
	}
}

void KEngine::KSystem::RenderSystem::renderPointLightShadowMap(
	ID3D11Device * device, ID3D11DeviceContext * context,

	Vector3 position, 
	ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport, ID3D11RasterizerState * cullBackFace, SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader, std::map<KEnum, Mesh>& meshes, EntityFactory & entityFactory)
{
	float viewportOriginalX = viewport.TopLeftX;
	float viewportOriginalWidth = viewport.Width;

	viewport.Width = (int)((viewportOriginalWidth-5) / 6.0f);
	context->RSSetState(cullBackFace);
	setRenderTarget(context, renderTargetView, depthStencilView, viewport);
	vertexShader.SetShader();
	fragmentShader.SetShader();
	m_cameraLight.setPosition(position);
	m_cameraLight.setQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), 3.14f / 2.0f));
	m_cameraLight.setProjParameters(3.14f/2.0f, 1,1, 0.1f, 1000.0f);
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_cameraLight.getProjMatrix());
	//std::cout << "VIEWPORT BEGIN " << viewport.TopLeftX << "," << viewport.TopLeftY << "(" << viewport.Width << "," << viewport.Height << std::endl;
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}
	//-x direction
	viewport.TopLeftX += 1 + (int)((viewportOriginalWidth - 5) / 6.0f);
	context->RSSetViewports(1, &viewport);
	m_cameraLight.setQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), -3.14f / 2.0f));
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}

	//Y direction
	viewport.TopLeftX += 1 + (int)((viewportOriginalWidth - 5) / 6.0f);
	context->RSSetViewports(1, &viewport);
	m_cameraLight.setQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), -3.14f / 2.0f));
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}

	//-Y direction
	viewport.TopLeftX += 1 + (int)((viewportOriginalWidth - 5) / 6.0f);
	context->RSSetViewports(1, &viewport);
	m_cameraLight.setQuaternion(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 3.14f / 2.0f));
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}

	//z direction
	viewport.TopLeftX += 1 + (int)((viewportOriginalWidth - 5) / 6.0f);
	context->RSSetViewports(1, &viewport);
	m_cameraLight.setQuaternion(Quaternion::Identity);
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}

	//-Z direction
	viewport.TopLeftX += 1 + (int)((viewportOriginalWidth - 5) / 6.0f);
	context->RSSetViewports(1, &viewport);
	m_cameraLight.setQuaternion(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), 3.14f ));
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());

	//std::cout << "VIEWPORT END " << viewport.TopLeftX << "," << viewport.TopLeftY << "(" << viewport.Width << "," << viewport.Height << std::endl;
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}


	viewport.TopLeftX = viewportOriginalX;
	viewport.Width = viewportOriginalWidth;
}
void KEngine::KSystem::RenderSystem::renderSpotLightShadowMap(
	ID3D11Device * device, ID3D11DeviceContext * context, 

	Vector3 position, Quaternion rotation, float fov,
	ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport, ID3D11RasterizerState * cullBackFace, SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader, std::map<KEnum, Mesh>& meshes, EntityFactory & entityFactory)
{
	//float colorClean[4] = { 0.1f,0.1f,0.1f,1 };
	//context->ClearRenderTargetView(renderTargetView, colorClean);
	//context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(cullBackFace);
	setRenderTarget(context, renderTargetView, depthStencilView, viewport);
	vertexShader.SetShader();
	fragmentShader.SetShader();
	m_cameraLight.setPosition(position);
	m_cameraLight.setQuaternion(rotation);
	m_cameraLight.setProjParameters(fov,viewport.Width,viewport.Height, 0.1f, 1000.0f);
	setMatrix(&vertexShader, "view", m_cameraLight.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_cameraLight.getProjMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED)
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if (lightComponent != 0) {
			//There is light.
			//For now don't do anything and just skip this case
			continue;
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
	}
}



void RenderSystem::update(std::vector<Entity> &entities, float time)
{
	for (int i = 0; i < m_components.size(); i++) {
		Entity& entity = entities[m_components[i].entityIndex];
		if (entity.m_transform3D->isDirty) {
			m_components[i].setPosition(entity.m_transform3D->position);
			m_components[i].setScale(entity.m_transform3D->size);
			m_components[i].setRotation(entity.m_transform3D->rotation);
			//std::cout << i << " : " << entities[m_components[i].entityIndex].m_transform3D->position.x << " , " <<
			//	entities[m_components[i].entityIndex].m_transform3D->position.y << " , " << entities[m_components[i].entityIndex].m_transform3D->position.z << "\n";
			//system("pause");
		}
	}
}


void KEngine::KSystem::RenderSystem::OnResize(int targetFrameWidth, int targetFrameHeight)
{
	m_camera.setProjParameters((3.14f / 2.0f), targetFrameWidth, targetFrameHeight, 0.1f, 1000.0f);
}