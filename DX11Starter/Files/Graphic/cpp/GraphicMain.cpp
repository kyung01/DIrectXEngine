#include "Graphic\GraphicMain.h"
#include <string>
#include "Graphic\RenderInstruction.h"
//trying to save the texture

#include <ScreenGrab.h>

#include <wincodec.h>

//end
using namespace NGraphic;

static int SIZE_LIGHT_TEXTURE = 512;
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
	context->OMSetRenderTargets(0,NULL, NULL);

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
	m_probeStagingTexutre.init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE * 2);
	return true;
}
void GraphicMain::updateBufferLightPrameter(
	ID3D11DeviceContext *context, ID3D11Buffer* buffer, std::list<std::shared_ptr<NScene::Light>>& lights)
{
	
}

void GraphicMain::updateLightAtlas(std::list<std::shared_ptr<NScene::Light>> &lights)
{
	float size = 1;
	
	m_atlasSlicer->clear();
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;
		//auto &info =  m_lightInfos[light.m_id];
		//0 spotlight
		//1 pointlight
		if (light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT) {

			if (!m_atlasSlicer->getRoom(
				light.m_atlasTopLeftX, light.m_atlasTopLeftY, 
				light.m_atlasViewportWidth, light.m_atlasViewportHeight, (int)size, (int)size)) {
				std::cout << "GraphicMain::updateLightAtlas-> Updating Light Atals Failed.\n";
				system("pause");
			}
			else {
				//size += 1.9f;
				//std::cout << "GraphicMain::updateLightAtlas-> Received available space\n";
				//std::cout <<"CREATED VIEWPORT : " << info.topLeftX << " , " << info.topLeftY<< " , " << info.viewportWidth << " , " << info.viewportHeight<<"\n";

				//success

			}
		}
		else {
			if (!m_atlasSlicer->getRoom(

				light.m_atlasTopLeftX, light.m_atlasTopLeftY,
				light.m_atlasViewportWidth, light.m_atlasViewportHeight, 
				size * 6, size)) {
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
	m_depthTextures[DEPTH_LIGHT_ATLAS]->clear(context);
	//m_depthTextures[DEPTH_LIGHT_ATLAS]->clear(context);

	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		auto &light = **it;
		//auto &lightInfo = m_lightInfos[light.m_id];
		if(light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT)
			RenderInstruction::RENDER_LIGHT_ATLAS_SPOT(
			device, context, asset,
			*m_renderTextures[TARGET_LIGHT_ATLAS], *m_depthTextures[DEPTH_LIGHT_ATLAS],
			scene,

			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix((**it).m_atlasViewportWidth, (**it).m_atlasViewportHeight),
			(**it).m_atlasTopLeftX, (**it).m_atlasTopLeftY, (**it).m_atlasViewportWidth, (**it).m_atlasViewportHeight);
		else {
			auto pointLight = static_cast<NScene::PointLight*>(&light);
			//(**it).setFOV(3.14f / 2.0f + 0.11f);
			RenderInstruction::RENDER_LIGHT_ATLAS_POINT(
				device, context, asset,
				*m_renderTextures[TARGET_LIGHT_ATLAS], *m_depthTextures[DEPTH_LIGHT_ATLAS],
				scene,

				worldMatrix, 
				pointLight->getMatrixXPlus(), pointLight->getMatrixXMinus(),
				pointLight->getMatrixYPlus(), pointLight->getMatrixYMinus(), 
				pointLight->getMatrixZPlus(), pointLight->getMatrixZMinus(), 
				(**it).getProjectionMatrix((**it).m_atlasViewportWidth /6, (**it).m_atlasViewportWidth / 6),
				(**it).m_atlasTopLeftX, (**it).m_atlasTopLeftY, (**it).m_atlasViewportWidth, (**it).m_atlasViewportHeight);
			//(**it).setFOV(3.14f / 2.0f);
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
		fov = 3.14f / 2.0f, // *((float)width / height),
		NEAR_DISTANCE(0.1),
		FAR_DISTANCE(500),
		X_DIIVIDE(10),
		Y_DIVIDE(10),
		Z_DIVIDE(10),
		CLUSTER_ITEM_SIZE(3200);
	this->m_width = width;
	this->m_height = height;
	m_rsm_flux_eye_perspective_width = textureIndirectLightWidth;
	m_rsm_flux_eye_perspective_height = textureIndirectLightHeight;
	m_frustumLight.init((float)m_width / m_height, NEAR_DISTANCE, FAR_DISTANCE, X_DIIVIDE, Y_DIVIDE, Z_DIVIDE);
	m_frustumProbe.init(1.0f, NEAR_DISTANCE, FAR_DISTANCE, X_DIIVIDE, Y_DIVIDE, Z_DIVIDE);
	m_bufferDataTranslator = std::make_shared<BufferDataTranslator>(X_DIIVIDE* Y_DIVIDE* Z_DIVIDE, CLUSTER_ITEM_SIZE,256,256,256);
	//m_lightBuffer = std::make_shared<NBuffer::KDynamicBuffer<NBuffer::LightParameter>>(10);

	if (
		!initTextures(device,context,width,height, textureIndirectLightWidth, textureIndirectLightHeight)
		
		) return false;

	orthoView = DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up);
	orthoMVP = DirectX::XMMatrixMultiply( 
		DirectX::XMMatrixLookToLH(Vector3(0, 0, 1), Vector3(0, 0, -1), Vector3::Up) , 
		DirectX::XMMatrixOrthographicLH(1,1,0.1,100) );

	return true;
}

//If there are new lights probes, then process them; make them ready for rendering process
//for lights, recalculate light boundaries according to the changed view projection
void GraphicMain::update(
	ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene)
{
	updateUnInitializedObjects(device, context, deltaTime, totalTime, asset, scene);
	updateFrustum(device, context, deltaTime, totalTime, asset,
		m_frustumLight,
		scene.m_camMain.getViewMatrix(), scene.objs_lights);



	beginRendering(context);
	renderLightAtlas(device, context, asset, scene);


	endRendering(context);




}

void GraphicMain::updateUnInitializedObjects(ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene)
{
	if (!scene.objs_lightsNotReady.empty()) {

		for (auto it = scene.objs_lightsNotReady.begin(); it != scene.objs_lightsNotReady.end(); it++) {
			(**it).m_deferredTexture = std::shared_ptr<RenderTexture>(new RenderTexture());
			(**it).m_deferredDepth = std::shared_ptr<DepthTexture>(new DepthTexture());
			(**it).m_deferredTexture->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			(**it).m_deferredDepth->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
		}
		scene.objs_lights.insert(scene.objs_lights.begin(), scene.objs_lightsNotReady.begin(), scene.objs_lightsNotReady.end());
		scene.objs_lightsNotReady.clear();
	}
	if (!scene.m_probesNotReady.empty()) {

		for (auto it = scene.m_probesNotReady.begin(); it != scene.m_probesNotReady.end(); it++) {
			(**it).m_deferredTexture = std::shared_ptr<RenderTexture>(new RenderTexture());
			(**it).m_deferredTexture->init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE * 2);
			(**it).m_deferredDepth = std::shared_ptr<DepthTexture>(new DepthTexture());
			(**it).m_deferredDepth->init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE * 2 );
		}
		scene.m_probes.insert(scene.m_probes.begin(), scene.m_probesNotReady.begin(), scene.m_probesNotReady.end());
		scene.m_probesNotReady.clear();
	}
}
Vector3 GraphicMain::getSpearNormal(int face, float pixelSize,float u, float v)
{
	Vector3 normal;
	float
		xDir = -1 + (u * 2) + pixelSize,
		yDir = -1 + (v * 2) + pixelSize;
	
	switch (face)
	{
	case 0: normal = Vector3(+1, +yDir, -xDir); break; // +X
	case 1: normal = Vector3(-1, +yDir, +xDir); break; // -X
	case 2: normal = Vector3(+xDir, +1, -yDir); break; // +Y
	case 3: normal = Vector3(+xDir, -1, +yDir); break; // -Y
	case 4: normal = Vector3(+xDir, yDir, +1); break; // +Z
	case 5: normal = Vector3(-xDir, yDir, -1); break; // -Z
	}
	normal.Normalize();
	return normal;
}
float AreaElement(float x, float y)
{
	return atan2(x * y,sqrt(x * x + y * y + 1));
}
float getSolidAngle(float U, float V, int a_Size)
{
	//scale up to [-1, 1] range (inclusive), offset by 0.5 to point to texel center.
	//float U = (2.0f * ((float)a_U + 0.5f) / (float)a_Size) - 1.0f;
	//float V = (2.0f * ((float)a_V + 0.5f) / (float)a_Size) - 1.0f;

	float InvResolution = (2.0f / a_Size) * 0.5f;

	float x0 = U - InvResolution;
	float y0 = V - InvResolution;
	float x1 = U + InvResolution;
	float y1 = V + InvResolution;
	float SolidAngle = AreaElement(x0, y0) - AreaElement(x1, y0) + AreaElement(x1, y1) - AreaElement(x0, y1);

	return SolidAngle;
}
void addColor(Vector3 *coefficients, float &solidAngleTotal, float u, float v, int faceDivisionCount, Vector3 normal ,Vector3 color ) {

	const float A0 = 1.0f;// 3.141593f;
	const float A1 = 1.0f;// 2.095395f; // Stick with 1.0 for all of these!!!
	const float A2 = 1.0f; //0.785398f;
	float domega = getSolidAngle(u, v, faceDivisionCount);
	solidAngleTotal += domega;
	coefficients[0] += 0.282095f * A0 * domega * color;

	// Band 1
	coefficients[1] += 0.488603f * normal.y * A1 * domega * color;
	coefficients[2] += 0.488603f * normal.z * A1 * domega * color;
	coefficients[3] += 0.488603f * normal.x * A1 * domega * color;

	// Band 2
	coefficients[4] += 1.092548f * normal.x * normal.y * A2 * domega * color;
	coefficients[5] += 1.092548f * normal.y * normal.z * A2 * domega * color;
	coefficients[6] += 0.315392f * (3.0f * normal.z * normal.z - 1.0f) * A2 * domega * color;
	coefficients[7] += 1.092548f * normal.x * normal.z * A2 * domega * color;
	coefficients[8] += 0.546274f * (normal.x * normal.x - normal.y * normal.y) * A2 * domega * color;
}

int test = 0;
void GraphicMain::updateProbes(
	ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene)
{
	

	//beginRendering(context);
	float depthMin = 0;
	float depthMax = 1.0f;
	for (auto it = scene.m_probes.begin(); it != scene.m_probes.end(); it++) {
		auto &probe = **it;
		float textureWidth = probe.m_deferredTexture->getWidth() / 6.0f;
		float textureHeight = textureWidth;

		auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
		auto viewMatirx = probe.getMatrixXMinus();

		DirectX::SimpleMath::Matrix projMatrix = DirectX::XMMatrixPerspectiveFovLH(
			3.14f / 2,		// Field of View Angle
			1.0f,		// Aspect ratio
			0.001, 1000.0f);
		//auto projMatrix = scene.m_camMain.getProjectionMatrix();

		probe.m_deferredTexture->clear(context, 0, 0, 0, 1);
		probe.m_deferredDepth->clear(context);

		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixXPlus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ 0.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixXPlus(),
			projMatrix,
			scene);
		
		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixXMinus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ textureWidth*1.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixXMinus(),
			projMatrix,
			scene);
		
		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixYPlus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ textureWidth*2.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixYPlus(),
			projMatrix,
			scene);
		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixYMinus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ textureWidth*3.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixYMinus(),
			projMatrix,
			scene);
		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixZPlus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ textureWidth*4.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixZPlus(),
			projMatrix,
			scene);

		updateFrustum(device, context, deltaTime, totalTime, asset,
			m_frustumProbe,
			probe.getMatrixZMinus(), scene.objs_lights);
		renderClusteredForward(device, context,
			probe.m_deferredTexture->getRenderTargetView(),
			probe.m_deferredDepth->getDepthStencilView(),
			{ textureWidth*5.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
			asset, m_frustumProbe,
			worldMatrix,
			probe.getMatrixZMinus(),
			projMatrix,
			scene);
		{
			Vector3* coefficients = new Vector3[9];
			float solidAngleTotal = 0;
			float gamma = 2.2f;
			HRESULT h;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			//don't copy resources for now
			//context->CopyResource(m_probeStagingTexutre.getShaderResource(), probe.m_deferredTexture->getShaderResource());
			h = context->Map(m_probeStagingTexutre.getShaderResource(), 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
			float* pointer = (float*)mappedResource.pData;
			//DirectX::D3DX11SaveTextureToFile
			for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
				for (float j = 0; j < SIZE_LIGHT_TEXTURE; j++)
					for (float i = 0; i < SIZE_LIGHT_TEXTURE; i ++)
					{
						int index  = j*SIZE_LIGHT_TEXTURE + i;
						float u = i / SIZE_LIGHT_TEXTURE;
						float v = j / SIZE_LIGHT_TEXTURE;
						//index++;
						Vector3 normal = getSpearNormal(faceIndex, 1.0f / SIZE_LIGHT_TEXTURE,u,v);
						Vector3 color(pow(pointer[index * 4],gamma), pow(pointer[index * 4+1],gamma), pow(pointer[index * 4+2],gamma));

						//std::cout << i << " Noraml " << j << " : " << normal.x << " , " << normal.y << " , " << normal.z << "\n";
						//std::cout << i << " Color " << j << " : " << color.x << " , " << color.y << " , " << color.z << "\n";
						addColor(coefficients, solidAngleTotal,u,v, SIZE_LIGHT_TEXTURE, normal, color);
						
						//i++;

					}
			}
				std::cout << "SOLID ANGLE " << solidAngleTotal << "\n";
			for (int i = 0; i < 9; i++) {

				std::cout << "BEFORE " << i << " : " << coefficients[i].x << " , " << coefficients[i].y << " , " << coefficients[i].z << "\n";
				coefficients[i] *= (4 * 3.14f) / solidAngleTotal;
				std::cout << "AFTER "<<i << " : " << coefficients[i].x << " , " << coefficients[i].y << " , " << coefficients[i].z << "\n";
			}
			//float colorRed[] = { pow(0.5f,2.2),0,0,0.5f };
			int8_t  colorRed[] = { 50,100,0,255 };
			for (float j = SIZE_LIGHT_TEXTURE; j < SIZE_LIGHT_TEXTURE*2; j++)
				for (float i = 0; i < SIZE_LIGHT_TEXTURE*6; i++)
				{
					int index = ((j)*(SIZE_LIGHT_TEXTURE * 6) + i) * 4;
					//int index = i*4;
					colorRed[2] = i/(SIZE_LIGHT_TEXTURE * 6) * 255;
					//colorRed[0] = i/(SIZE_LIGHT_TEXTURE * 6.0f);
					memcpy((int8_t *)mappedResource.pData+ index, colorRed, sizeof(colorRed));

					//i++;

				}
			//system("pause");
			context->Unmap(m_probeStagingTexutre.getShaderResource(), 0);
			//SaveDDSTextureToFile(context, m_probeStagingTexutre.getShaderResource(),  L"SCREENSHOT.dds");
			//GUID_WICPixelFormat64bppRGBA
			SaveWICTextureToFile(context, m_probeStagingTexutre.getShaderResource(), GUID_ContainerFormatPng, L"SCREENSHOT.png");
			//SaveWICTextureToFile(context, m_probeStagingTexutre.getShaderResource(), GUID_ContainerFormatJpeg, L"SCREENSHOT.jpg");
			//std::cout << "RESULT \n";
			delete coefficients;
			//delete colorRed;
			DirectXUtility::HRESULT_CHECK(h);
			//system("pause");
		}
	}

	//endRendering(context);


}
/*
renderClusteredForward(this->device, this->context,
backBufferRTV, depthStencilView, viewport,
m_asset, it->scene);
*/
void GraphicMain::updateFrustum(
	ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, 
	Asset & asset,
	NGraphic::NFrustum::Frustum &frustum,
	DirectX::SimpleMath::Matrix camViewMatrix,
	std::list < std::shared_ptr< NScene::Light> > lights)
{
	frustum.testBegin();
	int index = 0;
	for (auto it = lights.begin(); it != lights.end(); it++, index++) {
		auto &light = **it;
		Vector3 pos = XMVector3Transform(light.m_pos, camViewMatrix);
		Vector3 posDirLook = XMVector3Transform(light.m_pos + light.m_dirLook, camViewMatrix);
		Vector3 dir = posDirLook - pos;

		dir.Normalize();

		switch (light.m_lightType) {
		case NScene::POINTLIGHT:
			frustum.testPointlight(index, pos, light.m_lightDistance);
			break;
		case NScene::SPOTLIGHT:
			frustum.testSpotlight(index, pos, dir, light.m_lightDistance, light.getFOV());
			break;
		}
	}

	m_bufferDataTranslator->translate(lights);
	m_bufferDataTranslator->translate(frustum.m_clusters);
	m_bufferDataTranslator->transfer(
		context,
		asset.m_shadersFrag[RENDER_TEST]->GetBuffer(0), asset.m_shadersFrag[RENDER_TEST]->GetBuffer(1),
		asset.m_shadersFrag[RENDER_TEST]->GetBuffer(2), 0, 0);


	updateLightAtlas(lights);
}








void GraphicMain::renderClusteredForwardRendering(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene,
	ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView, D3D11_VIEWPORT & viewport,
	RenderTexture & textureAtlas, DepthTexture & depthAtlas,
	Matrix & matWorld, Matrix& matView, Matrix & matProj,
	int frustumSizeX, int frustumSizeY, int frustumSizeZ,
	float frustumFov, float frustumNear, float frustumFar,float frustumWidthOverHeight
)
{
	{
		DirectX::XMFLOAT4X4 MAT_TEMP;
		DirectX::XMStoreFloat4x4(&MAT_TEMP, XMMatrixTranspose(matView));
		asset.m_shadersFrag[RENDER_TEST]->SetMatrix4x4("eyeViewMatrix", MAT_TEMP);
		asset.m_shadersFrag[RENDER_TEST]->SetInt("frustumX", frustumSizeX);
		asset.m_shadersFrag[RENDER_TEST]->SetInt("frustumY", frustumSizeY);
		asset.m_shadersFrag[RENDER_TEST]->SetInt("frustumZ", frustumSizeZ);
		asset.m_shadersFrag[RENDER_TEST]->SetFloat("frustumSizeRatio", frustumWidthOverHeight);
		asset.m_shadersFrag[RENDER_TEST]->SetFloat("frustumNear",frustumNear );
		asset.m_shadersFrag[RENDER_TEST]->SetFloat("frustumFar", frustumFar);
		asset.m_shadersFrag[RENDER_TEST]->CopyAllBufferData();
	}
	//Clear once before the new scene
	m_renderTextures[TARGET_TEST]->clear(context, 0, 0, 0, 0);
	m_depthTextures[DEPTH_TEST]->clear(context);
	//Render
	RenderInstruction::RENDER_TEST(device, context, asset, scene.objs_solid,
		renderTargetView, depthStencilView, viewport,
		matWorld,matView,matProj , depthAtlas, textureAtlas, 0);

}

void NGraphic::GraphicMain::renderClusteredForward(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT viewport,
	Asset& asset,
	NGraphic::NFrustum::Frustum &frustum,
	SimpleMath::Matrix worldMatrix,
	SimpleMath::Matrix viewMatirx,
	SimpleMath::Matrix projMatrix,
	NScene::Scene& scene
	){
	//auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	//auto viewMatirx = scene.m_camMain.getViewMatrix();
	//auto projMatrix = scene.m_camMain.getProjectionMatrix(m_width,m_height);
		
	beginRendering(context);
	//renderLightAtlas(device, context, asset, scene);
	renderClusteredForwardRendering(
		device, context, asset, scene,
		target, targetDepth, viewport,
		*m_renderTextures[TARGET_LIGHT_ATLAS], 
		*m_depthTextures[DEPTH_LIGHT_ATLAS],
		worldMatrix, viewMatirx, projMatrix,

		(int)frustum.m_size.x,
		(int)frustum.m_size.y,
		(int)frustum.m_size.z,
		frustum.m_fov,
		frustum.m_near,
		frustum.m_far,
		frustum.m_widthOverHeight
	);
	endRendering(context);

	m_renderTextureDummy.setRenderTargetView(target, viewport);
	m_depthTextureDummy.setDepthStencilView(targetDepth);
	DirectX::SimpleMath::Matrix matSceneMvpFirstPerson = projMatrix *
		viewMatirx * worldMatrix;


	if (true) {
		beginRendering(context);
		m_renderTextureDummy.setRenderTarget(context, m_depthTextureDummy.getDepthStencilView());
		//context->ClearDepthStencilView(targetDepth, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		auto skyVS = asset.m_shadersVert[RENDER_SKYBOX].get();
		auto skyPS = asset.m_shadersFrag[RENDER_SKYBOX].get();

		DirectX::XMFLOAT4X4  view, projection;

		//DirectX::XMStoreFloat4x4(&mvp, XMMatrixTranspose(matSceneMvpFirstPerson)); // Transpose for HLSL!
		//DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(viewMatirx)); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!


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



	return;

}

void NGraphic::GraphicMain::renderDeffered(

	ID3D11Device * device, ID3D11DeviceContext * context,
	ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT & viewport,
	Asset& asset, NGame::Context &game){

	NScene::Scene & scene = *game.m_scene;

	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	auto worldMatrixFrustum = DirectX::SimpleMath::Matrix::CreateRotationX(3.14 / 2);
	auto viewMatirx = scene.m_camMain.getViewMatrix();
	auto projMatrix = scene.m_camMain.getProjectionMatrix(m_width, m_height);

	RenderInstruction::RENDER_WORLD_NORMAL_DIFFUSE(
		device, context, asset, scene,
		*m_renderTextures[TARGET_WORLD],
		*m_renderTextures[TARGET_NORMAL],
		*m_renderTextures[TARGET_DIFFUSE],
		*m_renderTextures[TARGET_PROPERTY],
		*m_depthTextures[DEPTH_WORLD],
		worldMatrix);


	m_renderTextures[TARGET_FINAL]->clear(context, 0, 0, 0, 1);



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
				device, context, asset,
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
				light.m_pos, light.m_dirLook, light.getLightColor(), light.getFOV() * RATIO_LIGHT_INNER, light.getFOV(), 1.0f,
				*m_renderTextures[TARGET_WORLD],
				*m_renderTextures[TARGET_LIGHTSHAFT_FRONT],
				*m_renderTextures[TARGET_LIGHTSHAFT_BACK],
				*m_lightInfos[it->get()->m_id].position, lightMVP, light.getFOV());
			endRendering(context);

		}

		if (false) {
			m_depthTextureDummy.clear(context);
		}

	}
	if (true) {
		beginRendering(context);
		m_depthTextures[DEPTH_FINAL]->clear(context);
		RenderInstruction::RENDER_DEBUG(
			device, context, asset,
			*m_renderTextures[TARGET_FINAL], *m_depthTextures[DEPTH_FINAL],
			game, scene, m_frustumLight,
			*m_depthTextures[DEPTH_WORLD]);
		endRendering(context);
	}
}


