#include "RenderInstruction.h"
using namespace NGraphic;

D3D11_VIEWPORT RenderInstruction::VIEWPORT;
D3D11_VIEWPORT RenderInstruction::VIEWPORT_TEMP;
DirectX::XMFLOAT4X4 RenderInstruction::MAT_TEMP;
DirectX::XMFLOAT4X4 RenderInstruction::matWorld;
DirectX::XMFLOAT4X4 RenderInstruction::matView;
DirectX::XMFLOAT4X4 RenderInstruction::matProj;
void RenderInstruction::SET_MATRIX(ISimpleShader * shader, std::string name, XMMATRIX matrix)
{
	DirectX::XMStoreFloat4x4(&MAT_TEMP, XMMatrixTranspose(matrix));
	shader->SetMatrix4x4(name, MAT_TEMP);
}
void RenderInstruction::RENDER_LIGHTS(ID3D11DeviceContext * context, SimpleVertexShader& shaderVertSimpleColor, SimpleFragmentShader& DshaderFrag, Mesh& mesh, XMMATRIX& worldMatrix) {
	//NGraphic::Mesh& mesh = *asset.m_meshes[MESH_POINTLIGHT];
	DirectX::XMFLOAT4X4 matWorld;
	DirectX::XMStoreFloat4x4(&matWorld, XMMatrixTranspose( worldMatrix));
	shaderVertSimpleColor.SetMatrix4x4("world", matWorld);
	shaderVertSimpleColor.CopyAllBufferData();
	DshaderFrag.CopyAllBufferData();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh.getBufferVertexRef(), &stride, &offset);
	context->IASetIndexBuffer(mesh.getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		mesh.getBufferIndexCount(),
	0,\
	0);
}
/*
Require : world position shader, render frustum shader.
*/
void RenderInstruction::RENDER_FRUSTUM(ID3D11Device * device, 
	ID3D11DeviceContext * context, Asset & asset, Vector3 eyePosition, DirectX::XMMATRIX & worldMatrix, DirectX::SimpleMath::Matrix & viewMatrix, DirectX::SimpleMath::Matrix & projMatrix, 
	RenderTexture & renderTexture, DepthTexture & depthTexture, RenderTexture & renderTexture2, DepthTexture & depthTexture2)
{

	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[KEnum::RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[KEnum::RENDER_WORLD];
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
	RENDER_LIGHTS(context, shaderVert, shaderFrag, *asset.m_meshes[MESH_POINTLIGHT],worldMatrix);

	context->RSSetState(asset.RASTR_STATE_CULL_FRONT);
	renderTexture2.clear(context, 0, 0, 0, 0);
	depthTexture2.clear(context);
	renderTexture2.setRenderTarget(context, depthTexture2.getDepthStencilView());

	RENDER_LIGHTS(context, shaderVert, shaderFrag, *asset.m_meshes[MESH_POINTLIGHT], worldMatrix);
	//context->OMSetDepthStencilState(asset.DEPTH_STATE_SKYBOX, 0);
	{


		renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
		shaderVertSimpleColor.SetMatrix4x4("view", matView);
		shaderVertSimpleColor.SetMatrix4x4("proj", matProj);
		shaderFragSimpleColor.SetFloat3("eyePos", eyePosition);

		shaderVertSimpleColor.SetShader();
		shaderFragSimpleColor.SetShader();
		RENDER_LIGHTS(context, shaderVertSimpleColor, shaderFragSimpleColor, *asset.m_meshes[MESH_POINTLIGHT], worldMatrix);
	}

}

void NGraphic::RenderInstruction::RENDER_LIGHTSHAFT_SPOTLIGHT(
	ID3D11Device * device, ID3D11DeviceContext * context,
	Asset & asset,
	RenderTexture & renderTexture, DepthTexture & depthTexture,
	DirectX::XMMATRIX orthoMVP,
	Vector3 eyePos, Vector3 eyeLook,
	Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter, float lightDensity,
	RenderTexture & world,
	RenderTexture &frustumFront, RenderTexture & frustumBack,
	RenderTexture & lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV)
{


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
	shaderFrag.SetShaderResourceView("textureShadow", lightShadow.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureWorld", world.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureFrustumFront", frustumFront.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureFrustumBack", frustumBack.getShaderResourceView());
	shaderFrag.SetFloat("density", lightDensity);
	shaderFrag.SetFloat3("eyePos", eyePos);
	shaderFrag.SetFloat3("eyeLook", eyeLook);
	shaderFrag.SetFloat3("lightPos", lightPos);
	shaderFrag.SetFloat3("lightDir", lightDir);
	shaderFrag.SetFloat3("lightColor", Vector3(lightColor));
	shaderFrag.SetFloat("lightInner", lightInner);
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
}

void RenderInstruction::RENDER_DEBUG(
	ID3D11Device * device, ID3D11DeviceContext * context,
	Asset& asset,
	RenderTexture& renderTexture, DepthTexture& depthTexture,
	NGame::Context& game, NScene::Scene& scene, Frustum& m_frustum,
	DepthTexture& textureEyeDepth
	 ) {
	
	DirectX::XMFLOAT4X4 matStore;
	auto &shaderVert = *asset.m_shadersVert[RENDER_TRANSPARENT];
	auto &shaderFrag = *asset.m_shadersFrag[RENDER_TRANSPARENT];
	NGraphic::Mesh&		sphere = *asset.m_meshes[MESH_ID_SPHERE];
	NGraphic::Mesh&		box = *asset.m_meshes[MESH_ID_CUBE];
	NGraphic::MeshLine&	line = *asset.m_meshLine;
	NGraphic::MeshCube&	cube = *asset.m_meshCube;
	auto r = renderTexture.getRenderTargetView();
	auto &view = renderTexture.getViewport();
	//atuo d = depthTexture.getDepthStencilView();
	context->OMSetRenderTargets(1, &r, NULL);
	context->RSSetViewports(1, &view);



	//context->RSSetState(asset.RASTR_STATE_CULL_);
	context->OMSetBlendState(asset.BLEND_STATE_ADDITIVE, 0, 0xffffffff);

	DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matStore);
	DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matStore);
	shaderFrag.SetShaderResourceView("textureEyeDepth",textureEyeDepth.getShaderResourceView());
	shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
	shaderFrag.SetFloat("SCREEN_WIDTH", renderTexture.getWidth());
	shaderFrag.SetFloat("SCREEN_HEIGHT", renderTexture.getHeight());
	shaderVert.SetShader();
	shaderFrag.SetShader();


	//Render virtual objects in scale of light
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {

		Vector3 lightScale = it->get()->m_scale;
		it->get()->setScale(Vector3(it->get()->m_lightDistance * 2));

		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(



			(**it).getModelMatrix())); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat3("color", Vector3((**it).getLightColor()));


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

	float angle = -m_frustum.m_angle / 2;
	float angleIncrease = m_frustum.m_angle / m_frustum.m_division;

	if (false)for (auto it = m_frustum.planesX.begin(); it != m_frustum.planesX.end(); it++) {
		auto matRotation = DirectX::XMMatrixRotationY(angle);
		auto matScale = DirectX::XMMatrixScaling(0.051, 0.5f, 20);
		red += 0.2f;


		//DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(matRotation,)); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose(DirectX::XMMatrixMultiply(matScale, matRotation))); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat4("color", Vector4(red, 1, 1, 1.0));


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
		Vector3 color = (**it).getLightColor();
		shaderFrag.SetFloat4("color", Vector4(color.x, color.y, color.z, 1));


		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();


		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &bufferVertices, &stride, &offset);
		context->IASetIndexBuffer(bufferIndices, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			3 * 2 * 6,
			//cube.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
		it->get()->setScale(lightScale);
	}

	DirectX::XMStoreFloat4x4(&matStore, Matrix::Identity); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("world", matStore);

	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	context->OMSetBlendState(asset.BLEND_STATE_TRANSPARENT, 0, 0xffffffff);
	int index = 0;

	float randomSeed = 0;

	for (auto it = m_frustum.m_clusters.begin(); it != m_frustum.m_clusters.end(); it++, index++) {
		randomSeed++;
		if (it->light.size()) {
			auto frustum = asset.m_frustums[index];
			bufferVertices = frustum->getBufferVertices();
			bufferIndices = frustum->getBufferIndices();
			auto color = asset.getRandomColor(randomSeed);
			shaderFrag.SetFloat4("color", Vector4(color.x, color.y, color.z, 0.5f));


			shaderVert.CopyAllBufferData();
			shaderFrag.CopyAllBufferData();


			UINT stride = sizeof(VertexPosition);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, &bufferVertices, &stride, &offset);
			context->IASetIndexBuffer(bufferIndices, DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(
				frustum->getBufferIndexCount(),
				//cube.getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
				0,     // Offset to the first index we want to use
				0);    // Offset to add to each index when looking up vertices
		}
	}





	//context->RSSetState(asset.RASTR_WIREFRAME);

	if (false)for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		bufferVertices = line.getBufferVertices();
		bufferIndices = line.getBufferIndices();
		Vector3 lightScale = it->get()->m_scale;
		it->get()->setScale(Vector3::One);

		DirectX::XMStoreFloat4x4(&matStore, XMMatrixTranspose((**it).getModelMatrix())); // Transpose for HLSL!
		shaderVert.SetMatrix4x4("world", matStore);
		shaderFrag.SetFloat3("color", Vector3((**it).getLightColor()));


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

}


void RenderInstruction::RENDER_WORLD_NORMAL_DIFFUSE(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, 
	NScene::Scene & scene,
	RenderTexture & targetWorld,
	RenderTexture & targetNormal,

	RenderTexture & targetDiffuse,

	RenderTexture & targetProperty,
	DepthTexture & depthWorld,
	DirectX::SimpleMath::Matrix worldMatrix)
{
	DirectX::XMFLOAT4X4 matrixStore;
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[RENDER_WORLD_DIFFUSE_NORMAL_PROPERTY];

	targetWorld		.clear(context, 0, 0, 0, 0);
	targetNormal	.clear(context, 0, 0, 0, 0);
	targetDiffuse	.clear(context, 0, 0, 0, 0);
	targetProperty	.clear(context, 0, 0, 0, 0);
	depthWorld		.clear(context);
	ID3D11RenderTargetView *renderTargets[]{
		targetWorld.getRenderTargetView(),
		targetNormal.getRenderTargetView(),
		targetDiffuse.getRenderTargetView(),
		targetProperty.getRenderTargetView()
	};
	context->OMSetRenderTargets(4, renderTargets, depthWorld.getDepthStencilView());
	context->RSSetViewports(1, &targetWorld.getViewport());

	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("view", matrixStore);
	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(targetWorld.getWidth(), targetWorld.getHeight()))); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("proj", matrixStore);
	shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++) {
		NGraphic::NScene::Object& object = **it;
		NGraphic::Mesh& mesh = *asset.m_meshes[(*it)->m_meshId];
		auto modelWorldMatrix = DirectX::XMMatrixMultiply(worldMatrix, object.getModelMatrix());
		DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(object.getModelMatrix())); // Transpose for HLSL!
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
	
}

void NGraphic::RenderInstruction::RENDER_WORLD(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, 
	RenderTexture & renderTexture, DepthTexture & depthTexture,
	NScene::Scene & scene, 
	DirectX::SimpleMath::Matrix & worldMatrix, DirectX::SimpleMath::Matrix & viewMatrix, DirectX::SimpleMath::Matrix & projMatrix)
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
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[KEnum::RENDER_WORLD];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[KEnum::RENDER_WORLD];
	//renderTexture.clear(context, 0, 0, 0, 99999);

	context->RSSetState(asset.RASTR_STATE_CULL_BACK);
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	//depthTexture.clear(context);
	//render the front face first



	SET_MATRIX(&shaderVert, "view", viewMatrix);
	SET_MATRIX(&shaderVert, "proj", projMatrix);


	//DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(viewMatrix)); // Transpose for HLSL!
	//shaderVert.SetMatrix4x4("view", matrixStore);
	//DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!
	//shaderVert.SetMatrix4x4("proj", matrixStore);
	//shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	RENDER_OBJS;


}

void NGraphic::RenderInstruction::RENDER_DIRECT_LIGHT(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
	RenderTexture & renderTexture, DepthTexture & depthTexture, 
	XMMATRIX orthoMVP, 
	Vector3 eyePos, Vector3 lightPos, Vector3 lightDir, Vector3 lightColor, float lightInner, float lightOutter, 
	std::shared_ptr<RenderTexture> lightShadow, DirectX::XMMATRIX lightMVP, float lightFOV,
	RenderTexture &textureWorld, RenderTexture &textureNormal, RenderTexture &textureDiffuse)
{

	DirectX::XMFLOAT4X4 matrixStore;
	NGraphic::Mesh& mesh = *asset.m_meshes[MESH_ID_PLANE];
	SimpleVertexShader&		shaderVert = *asset.m_shadersVert[KEnum::RENDER_DIRECT_LIGHT];
	SimpleFragmentShader&	shaderFrag = *asset.m_shadersFrag[KEnum::RENDER_DIRECT_LIGHT];
	//renderTexture.clear(context, 0, 0, 0, 1);
	//depthTexture.clear(context);
	context->OMSetBlendState(asset.BLEND_STATE_ADDITIVE, 0, 0xffffffff);
	context->RSSetState(asset.RASTR_STATE_CULL_BACK);
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());

	DirectX::XMStoreFloat4x4(&matrixStore, XMMatrixTranspose(orthoMVP)); // Transpose for HLSL!
	shaderVert.SetMatrix4x4("matMVP", matrixStore);
	shaderFrag.SetSamplerState("samplerWrap", asset.m_samplers[SAMPLER_ID_WRAP]);
	shaderFrag.SetSamplerState("samplerBoarderZero", asset.m_samplers[SAMPLER_ID_BORDER_ZERO]);
	shaderFrag.SetShaderResourceView("textureWorld", textureWorld.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureDiffuse", textureDiffuse.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureShadow", lightShadow->getShaderResourceView());
	shaderFrag.SetFloat3("eyePos", eyePos);
	shaderFrag.SetFloat3("lightPos", lightPos);
	shaderFrag.SetFloat3("lightColor", Vector3(lightColor));
	shaderFrag.SetFloat3("lightDir", lightDir);
	shaderFrag.SetFloat("lightInner", lightInner);
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
	shaderFrag.SetShaderResourceView("textureWorld", 0);
	shaderFrag.SetShaderResourceView("textureNormal", 0);
	shaderFrag.SetShaderResourceView("textureDiffuse", 0);
	shaderFrag.SetShaderResourceView("textureShadow", 0);

}
/*
void RenderInstruction::RENDER_LIGHT_ATLAS(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
	NScene::Scene &scene,
	RenderTexture & renderTexture, DepthTexture & depthTexture,
	NScene::Light &light, float topLeftX, float topLeftY, float viewportWidth, float ViewportHeight)
{
}
*/
	

void RenderInstruction::RENDER_LIGHT_ATLAS_SPOT(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
	NScene::Scene &scene,
	RenderTexture & renderTexture, DepthTexture & depthTexture,
	DirectX::SimpleMath::Matrix& worldMatrix, DirectX::SimpleMath::Matrix& viewMatrix, DirectX::SimpleMath::Matrix& projMatrix,
	float topLeftX, float topLeftY, float viewportWidth, float ViewportHeight)
{
	VIEWPORT.MinDepth = 0;
	VIEWPORT.MaxDepth = 1.0;
	VIEWPORT.TopLeftX = topLeftX;
	VIEWPORT.TopLeftY = topLeftY;
	VIEWPORT.Width = viewportWidth;
	VIEWPORT.Height = ViewportHeight;


	auto & shaderFrag = *asset.m_shadersFrag[KEnum::RENDER_WORLD];
	auto & shaderVert = *asset.m_shadersVert[KEnum::RENDER_WORLD];

	SET_MATRIX(&shaderVert, "world", worldMatrix);
	SET_MATRIX(&shaderVert, "view", viewMatrix);
	SET_MATRIX(&shaderVert, "proj", projMatrix);

	VIEWPORT_TEMP = renderTexture.getViewport();
	renderTexture.setViewport(VIEWPORT);
	renderTexture.setRenderTarget(context, depthTexture.getDepthStencilView());
	renderTexture.setViewport(VIEWPORT_TEMP);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	shaderVert.CopyAllBufferData();
	shaderFrag.CopyAllBufferData();

	for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++) {
		\
			NGraphic::NScene::Object& obj = **it; \
			NGraphic::Mesh& mesh = *asset.m_meshes[obj.m_meshId]; \
			SET_MATRIX(&shaderVert, "world", obj.getModelMatrix()*worldMatrix);
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
}
/*
Render in order of +x -x +y -y  +z -z
*/
void RenderInstruction::RENDER_LIGHT_ATLAS_POINT(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset,
	NScene::Scene &scene,
	RenderTexture &renderTexture, DepthTexture & depthTexture,
	NScene::Light &light, float topLeftX, float topLeftY, float viewportWidth, float ViewportHeight)
{
}