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

void KEngine::KSystem::RenderSystem::init(int renderTargetWidth, int renderTargetHeight)
{
	m_camera.setProjParameters((3.14f / 2.0f), renderTargetWidth, renderTargetHeight, 0.1f, 1000.0f);
}

DirectX::SimpleMath::Matrix KEngine::KSystem::RenderSystem::getCameraViewMatrix()
{
	return m_camera.getViewMatrix();
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

void RenderSystem::render(
	ID3D11Device * device, ID3D11DeviceContext * context,
	ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView* depthStencilView, D3D11_VIEWPORT & viewport,
	ID3D11RasterizerState *cullBackFace,
	SimpleVertexShader & vertexShader, SimpleFragmentShader & fragmentShader,
	std::map<KEnum, Mesh> &meshes,
	KEngine::EntityFactory & entityFactory
)
{
	float colorClean[4] = { 0.1f,0.1f,0.1f,1 };
	context->ClearRenderTargetView(renderTargetView, colorClean);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(cullBackFace);
	setRenderTarget(context, renderTargetView, depthStencilView, viewport);
	vertexShader.SetShader();
	fragmentShader.SetShader();
	setMatrix(&vertexShader, "view", m_camera.getViewMatrix());
	setMatrix(&vertexShader, "proj", m_camera.getProjMatrix());
	for (auto it = m_components.begin(); it != m_components.end(); it++) {
		if (it->meshId == KEnum::UNDEFINED) 
			continue;
		Mesh& mesh = meshes.find(it->meshId)->second;
		setMatrix(&vertexShader, "world", it->getWorldMatrix());
		vertexShader.CopyAllBufferData();
		auto lightComponent = entityFactory.getEntity(it->entityIndex).m_lightComponent;
		if(lightComponent == 0)
			fragmentShader.SetFloat3("diffuseColor", Vector3());
		else
			fragmentShader.SetFloat3("diffuseColor", Vector3(1) );

		fragmentShader.CopyAllBufferData();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.getBufferIndexCount(), 0, 0);
		//std::cout << "RENDERING AT " << it->getPosition().x << " , " << it->getPosition().y << " , " << it->getPosition().z << "\n";
		//send the new world matrix information
		//renderMesh(context, mesh, 0, sizeof(Vertex));
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