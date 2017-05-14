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
	m_probeCubemapTarget.init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE);
	m_probeCubemapDepth.init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE);
	m_probePrebake.init(device, DXGI_FORMAT_R32G32B32A32_FLOAT, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE * 1);
	m_probeBaked.init(device, DXGI_FORMAT_R8G8B8A8_UNORM, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE * 2);
	return true;
}
void GraphicMain::updateBufferLightPrameter(
	ID3D11DeviceContext *context, ID3D11Buffer* buffer, std::list<std::shared_ptr<NScene::SpotLight>>& lights)
{
	
}

void GraphicMain::updateLightAtlas(std::list<std::shared_ptr<NScene::SpotLight>> &lights)
{
	float size = 2;
	
	m_atlasSlicer->clear();
	for (auto it = lights.begin(); it != lights.end(); it++) {
		auto &light = **it;
		//auto &info =  m_lightInfos[light.m_id];
		//0 spotlight
		//1 pointlight
		if (light.m_lightType == NScene::LIGHT_TYPE::SPOTLIGHT) {

			if (!m_atlasSlicer->getRoom(
				light.m_atlas.topLeftX, light.m_atlas.topLeftY,
				light.m_atlas.width, light.m_atlas.height, (int)size, (int)size)) {
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
				light.m_atlas.topLeftX, light.m_atlas.topLeftY,
				light.m_atlas.width, light.m_atlas.height,
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
void NGraphic::GraphicMain::renderProbe(
	ID3D11Device * device, ID3D11DeviceContext * context, Asset & asset, NScene::Scene & scene, NScene::Probe & probe, RenderTexture & m_deferredTexture, DepthTexture & m_deferredDepth)
{
	float depthMin = 0;
	float depthMax = 1.0f;
	float deltaTime = 0;
	float totalTime = 0;
	float textureWidth = m_deferredTexture.getWidth() / 6.0f;
	float textureHeight = textureWidth;

	auto worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	auto viewMatirx = probe.getMatrixXMinus();

	DirectX::SimpleMath::Matrix projMatrix = DirectX::XMMatrixPerspectiveFovLH(
		3.14f / 2,		// Field of View Angle
		1.0f,		// Aspect ratio
		0.001, 1000.0f);
	//auto projMatrix = scene.m_camMain.getProjectionMatrix();

	m_deferredTexture.clear(context, 0, 0, 0, 1);
	m_deferredDepth.clear(context);

	updateFrustum(device, context, deltaTime, totalTime, asset,
		m_frustumProbe,
		probe.getMatrixXPlus(), scene.objs_lights);
	renderClusteredForward(device, context,
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
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
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
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
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
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
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
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
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
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
		m_deferredTexture.getRenderTargetView(),
		m_deferredDepth.getDepthStencilView(),
		{ textureWidth*5.0f,0.0f, textureWidth ,textureHeight,depthMin,depthMax },
		asset, m_frustumProbe,
		worldMatrix,
		probe.getMatrixZMinus(),
		projMatrix,
		scene);
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

			worldMatrix, (**it).getViewMatrix(), (**it).getProjectionMatrix((**it).m_atlas.width, (**it).m_atlas.height),
			(**it).m_atlas.topLeftX, (**it).m_atlas.topLeftY, (**it).m_atlas.width, (**it).m_atlas.height);
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
				(**it).getProjectionMatrix((**it).m_atlas.width /6, (**it).m_atlas.height ),
				(**it).m_atlas.topLeftX, (**it).m_atlas.topLeftY, (**it).m_atlas.width, (**it).m_atlas.height);
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
			(**it).m_deferredTexture->init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE );
			(**it).m_deferredDepth = std::shared_ptr<DepthTexture>(new DepthTexture());
			(**it).m_deferredDepth->init(device, SIZE_LIGHT_TEXTURE * 6, SIZE_LIGHT_TEXTURE  );
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
	case 0: normal = Vector3(+1, yDir, -xDir); break; // +X
	case 1: normal = Vector3(-1, yDir, +xDir); break; // -X
	case 2: normal = Vector3(+xDir, +1, -yDir); break; // -Y
	case 3: normal = Vector3(+xDir, -1, +yDir); break; // +Y
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

	const float A0 =  1;//3.141593f;
	const float A1 =  1;//2.095395f; // Stick with 1.0 for all of these!!!
	const float A2 =  1;//0.785398f;
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
void getSH(float* HS, Vector3 normal)
{
	float Y00 = 0.282095f;
	float Y11 = 0.488603f * normal.x;
	float Y10 = 0.488603f * normal.z;
	float Y1_1 = 0.488603f * normal.y;
	float Y21 = 1.092548f * normal.x * normal.z;
	float Y2_1 = 1.092548f * normal.y * normal.z;
	float Y2_2 = 1.092548f * normal.y * normal.x;
	//float Y20 = 0.946176f * normal.z * normal.z - 0.315392f;
	float Y20 = 0.315392f * (3 * normal.z * normal.z - 1);// - 0.315392f;
	float Y22 = 0.546274f * (normal.x * normal.x - normal.y * normal.y);
	HS[0]=(Y00);
	HS[1]= (Y1_1);
	HS[2] =(Y10);
	HS[3] =(Y11);

	HS[4]=(Y2_2);
	HS[5] =(Y2_1);
	HS[6] =(Y20);
	HS[7]=(Y21);
	HS[8]=(Y22);
}
Vector3 getColor(Vector3* coef, float *vertexSH, Vector3 N )
{
	float gamma = 2.2;
	/*
	*
	var vertexSH = vertex.getSH();
	float a0 = 3.141593f;
	float a1 = 2.094395f;
	float a2 = 0.785398f;

	Vector3 color =
	a0 * vertexSH[0] * coefficients[0] +
	a1 * vertexSH[1] * coefficients[1] +
	a1 * vertexSH[2] * coefficients[2] +
	a1 * vertexSH[3] * coefficients[3] +
	a2 * vertexSH[4] * coefficients[4] +
	a2 * vertexSH[5] * coefficients[5] +
	a2 * vertexSH[6] * coefficients[6] +
	a2 * vertexSH[7] * coefficients[7] +
	a2 * vertexSH[8] * coefficients[8];
	vertex.setColor(new Color(color.x, color.y, color.z));
	* */
	float C1 = 0.429043f;
	float C2 = 0.511665f;
	float C3 = 0.743125f;
	float C4 = 0.886227f;
	float C5 = 0.247708f;

	auto L00 = coef[0];
	auto L1_1 = coef[1];
	auto L10 = coef[2];
	auto L11 = coef[3];
	auto L2_2 = coef[4];
	auto L2_1 = coef[5];
	auto L20 = coef[6];
	auto L21 = coef[7];
	auto L22 = coef[8];

	// constant term, lowest frequency //////
	Vector3 color = C4 * coef[0] +

		// axis aligned terms ///////////////////
		2.0f * C2 * coef[1] * N.y +
		2.0f * C2 * coef[2] * N.z +
		2.0f * C2 * coef[3] * N.x +

		// band 2 terms /////////////////////////
		2.0f * C1 * coef[4] * N.x * N.y +
		2.0f * C1 * coef[5] * N.y * N.z +
		C3 * coef[6] * N.z * N.z - C5 * coef[6] +
		2.0f * C1 * coef[7] * N.x * N.z +
		C1 * coef[8] * (N.x * N.x - N.y * N.y);


	color.x = pow(color.x, 1.0f / gamma);
	color.y = pow(color.y, 1.0f / gamma);
	color.z = pow(color.z, 1.0f / gamma);
	return color;

}
int test = 0;
void GraphicMain::updateProbes(
	ID3D11Device * device, ID3D11DeviceContext * context, float deltaTime, float totalTime, Asset & asset, NScene::Scene & scene)
{
	

	//beginRendering(context);
	m_probeCubeArray.release();
	int probeCount = scene.m_probes.size();

	D3D11_SUBRESOURCE_DATA* pSubResourceDataCubeArray = new D3D11_SUBRESOURCE_DATA[6 * probeCount];
	std::vector < std::vector<Vector4>> dataCubeArray(6 * probeCount, std::vector<Vector4>(SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE));

	int probeIndex = 0;
	for (int i = 0; i < 6 * probeCount; i++) {
		for (int j = 0; j < SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE; j++) {
			dataCubeArray[i][j] = Vector4(0, 1, 0, 1);
		}
	}
	for (auto it = scene.m_probes.begin(); it != scene.m_probes.end(); it++, probeIndex++) {

		renderProbe(device, context, asset, scene, **it, m_probeCubemapTarget, m_probeCubemapDepth );
		auto probeShaderResource = m_probeCubemapTarget.getShaderResource();
		{ 


			Vector3* coefficientsChannels = new Vector3[9];
			int8_t* pixels = new int8_t[SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE*6 *4];
			float* coefficientsVertex = new float[9];
			float solidAngleTotal = 0;
			float gamma = 2.2f;
			HRESULT h;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			//don't copy resources for now
			context->CopyResource(m_probePrebake.getShaderResource(), probeShaderResource);
			h = context->Map(m_probePrebake.getShaderResource(), 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
			float* pointer = (float*)mappedResource.pData;
			//DirectX::D3DX11SaveTextureToFile
			for (int i = 0; i < SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE * 6 * 4; i++) {
				pixels[i] = pointer[i]*255;
			}
			for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
				for (float j = 0; j < SIZE_LIGHT_TEXTURE; j++)
					for (float i = 0; i < SIZE_LIGHT_TEXTURE; i ++)
					{
						int index  = j* (SIZE_LIGHT_TEXTURE * 6) + (SIZE_LIGHT_TEXTURE*faceIndex + i);

						dataCubeArray[probeIndex *(6)+ faceIndex][j *SIZE_LIGHT_TEXTURE + i] = Vector4(pointer[index * 4], pointer[index * 4+1], pointer[index * 4+2], 1);
						float u = i / SIZE_LIGHT_TEXTURE;
						float v = j / SIZE_LIGHT_TEXTURE;
						//index++;
						Vector3 normal = getSpearNormal(faceIndex, 1.0f / SIZE_LIGHT_TEXTURE,u,v);
						//Vector3 color(pointer[index * 4], pointer[index * 4 + 1], pointer[index * 4 + 2]);
						Vector3 color(pow(pointer[index * 4], gamma), pow(pointer[index * 4 + 1], gamma), pow(pointer[index * 4 + 2], gamma));

						//std::cout << i << " Noraml " << j << " : " << normal.x << " , " << normal.y << " , " << normal.z << "\n";
						//std::cout << i << " Color " << j << " : " << color.x << " , " << color.y << " , " << color.z << "\n";
						addColor(coefficientsChannels, solidAngleTotal,u,v, SIZE_LIGHT_TEXTURE, normal, color);
						
						//i++;

					}
			}
			context->Unmap(m_probePrebake.getShaderResource(), 0);
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			h = context->Map(m_probeBaked.getShaderResource(), 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
				std::cout << "SOLID ANGLE " << solidAngleTotal << "\n";
			for (int i = 0; i < 9; i++) {

				std::cout << "BEFORE " << i << " : " << coefficientsChannels[i].x << " , " << coefficientsChannels[i].y << " , " << coefficientsChannels[i].z << "\n";
				coefficientsChannels[i] *= (4 * 3.14f) / solidAngleTotal;
				std::cout << "AFTER "<<i << " : " << coefficientsChannels[i].x << " , " << coefficientsChannels[i].y << " , " << coefficientsChannels[i].z << "\n";
			}
			//system("pause");
			//float colorRed[] = { pow(0.5f,2.2),0,0,0.5f };
			int8_t  colorRed[] = { 50,100,0,255 };
			memcpy((int8_t *)mappedResource.pData, pixels, sizeof(int8_t) *SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE * 6 * 4);
			std::vector < std::vector<Vector4>> diffuseMap(6, std::vector<Vector4>(SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE));
			float* someData = new float[SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE*6*4];
			for (int i = 0; i < SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE * 6 * 4; i++) {
				someData[i] = 1;
			}
			D3D11_SUBRESOURCE_DATA pData[6];

			std::vector<Vector4> d[6]; // 6 images of type vector4b = 4 * unsigned char
			for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
			{
				d[cubeMapFaceIndex].resize(SIZE_LIGHT_TEXTURE * SIZE_LIGHT_TEXTURE);

				// fill with red color  
				std::fill(
					d[cubeMapFaceIndex].begin(),
					d[cubeMapFaceIndex].end(),
					Vector4(1, 0, 0, 0));

			}

			for (int faceIndex = 0; faceIndex < 6; faceIndex++) {

				float someColor = 0;
				for (float j = 0; j < SIZE_LIGHT_TEXTURE ; j++)
					for (float i = 0; i < SIZE_LIGHT_TEXTURE ; i++) {
						someColor += 0.01f;
						float u = i / SIZE_LIGHT_TEXTURE;
						float v = 1-j / SIZE_LIGHT_TEXTURE;
						auto normal = getSpearNormal(faceIndex, 1.0f / SIZE_LIGHT_TEXTURE, u, v);
						getSH(coefficientsVertex, normal);
						//std::cout << "indexs" << "\n";
						for (int shIndex = 0; shIndex < 9; shIndex++) {
							//std::cout << coefficientsVertex[shIndex] << "\n";
						}
						Vector3 color = getColor(coefficientsChannels, coefficientsVertex, normal);
						someData[
							(

							(int)faceIndex *SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE + (int)i + (int)j* SIZE_LIGHT_TEXTURE
								)*4
						] = color.x;
						someData[


							(

								(int)faceIndex *SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE + (int)i + (int)j* SIZE_LIGHT_TEXTURE
								) * 4
								+1
						] = color.y;
						someData[


							(

								(int)faceIndex *SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE + (int)i + (int)j* SIZE_LIGHT_TEXTURE
								) * 4
								+2
						] = color.z;

						diffuseMap[faceIndex][j*SIZE_LIGHT_TEXTURE + i].x = color.x;
						diffuseMap[faceIndex][j*SIZE_LIGHT_TEXTURE + i].y = color.y;
						diffuseMap[faceIndex][j*SIZE_LIGHT_TEXTURE + i].z = color.z;
						diffuseMap[faceIndex][j*SIZE_LIGHT_TEXTURE + i].w = 1;
						
						//diffuseMap[faceIndex][j*SIZE_LIGHT_TEXTURE + i] = Vector4(color.x, color.y, color.z, 1);
						d[faceIndex][j*SIZE_LIGHT_TEXTURE + i] = Vector4(color.x, color.y, color.z, 1);
						colorRed[0] = color.x*255;
						colorRed[1] = color.y * 255;
						colorRed[2] = color.z * 255;
						//std::cout << " NORMAL " << normal.x << " , " << normal.y << " , " << normal.z << " \n";
						//std::cout << "COLOR HERE " << faceIndex << " : " << i << " , " << j << " : " << color.x << " , " << color.y << " , " << color.z << " \n";
						int index = (SIZE_LIGHT_TEXTURE + j) * (SIZE_LIGHT_TEXTURE*6) + (SIZE_LIGHT_TEXTURE*faceIndex+ i);
						index *= 4;
						memcpy((int8_t *)mappedResource.pData + index, colorRed, sizeof(colorRed));
					}

				//pData[faceIndex].pSysMem = &d[faceIndex][0];// description.data;
				//pData[faceIndex].pSysMem = &someData[faceIndex*(SIZE_LIGHT_TEXTURE*SIZE_LIGHT_TEXTURE) * 4];// description.data;
				pData[faceIndex].pSysMem = &diffuseMap[faceIndex][0];// description.data;
				pData[faceIndex].SysMemPitch = (SIZE_LIGHT_TEXTURE * 4)*sizeof(float);
				pData[faceIndex].SysMemSlicePitch = 0;
			}

			context->Unmap(m_probeBaked.getShaderResource(), 0);
			m_isProbeReady = true;
			//system("pause");
			//SaveDDSTextureToFile(context, m_probeStagingTexutre.getShaderResource(),  L"SCREENSHOT.dds");
			//GUID_WICPixelFormat64bppRGBA
			SaveWICTextureToFile(context, m_probeBaked.getShaderResource(), GUID_ContainerFormatPng, L"SCREENSHOT.png");
			//SaveWICTextureToFile(context, m_probeStagingTexutre.getShaderResource(), GUID_ContainerFormatJpeg, L"SCREENSHOT.jpg");
			//std::cout << "RESULT \n";
			delete coefficientsChannels;
			delete coefficientsVertex;
			delete pixels;
			delete someData;
			//delete colorRed;
			DirectXUtility::HRESULT_CHECK(h);
			//system("pause");
		}
	}

	for (int faceIndex = 0; faceIndex < 6 * probeCount; faceIndex++) {
		pSubResourceDataCubeArray[faceIndex].pSysMem = &dataCubeArray[faceIndex][0];// description.data;
		pSubResourceDataCubeArray[faceIndex].SysMemPitch = (SIZE_LIGHT_TEXTURE * 4) * sizeof(float);
		pSubResourceDataCubeArray[faceIndex].SysMemSlicePitch = 0;
	}



	m_probeCubeArray.initCubeArray(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, probeCount, pSubResourceDataCubeArray);

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
	std::list < std::shared_ptr< NScene::SpotLight> > lights)
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
	if(m_isProbeReady)
		RenderInstruction::RENDER_TEST(device, context, asset,
			renderTargetView, depthStencilView, viewport,
			matWorld, matView, matProj, depthAtlas,textureAtlas, m_probeCubeArray.getShaderResourceView(),
			0,
			scene.m_camMain.m_pos,
			SIZE_LIGHT_TEXTURE,
			scene.objs_solid,
			scene.objs_lights,
			scene.m_probes);
	else 

		RenderInstruction::RENDER_TEST(device, context, asset,
			renderTargetView, depthStencilView, viewport,
			matWorld, matView, matProj, depthAtlas, textureAtlas,  textureAtlas.getShaderResourceView(),
			0,
			scene.m_camMain.m_pos,
			SIZE_LIGHT_TEXTURE,
			scene.objs_solid,
			scene.objs_lights,
			scene.m_probes);

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


		NScene::SpotLight& light = **it;
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


