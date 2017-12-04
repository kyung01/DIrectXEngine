#include <Engine\Componenets\ProbeComponent.h>
using namespace KEngine::KComponent;

ProbeComponent::ProbeComponent() :
	isNeedUpdate(true)
{
	initCoefficients();
}

void KEngine::KComponent::ProbeComponent::initRenderTexture(ID3D11Device *device, int size)
{
	type = PROBE_TYPE::REFLECTION;
	xPlus = std::make_unique<NDirectX::RenderTexture>();
	xMinus = std::make_unique<NDirectX::RenderTexture>();
	yPlus = std::make_unique<NDirectX::RenderTexture>();
	yMinus = std::make_unique<NDirectX::RenderTexture>();
	zPlus = std::make_unique<NDirectX::RenderTexture>();
	zMinus = std::make_unique<NDirectX::RenderTexture>();
	xPlus->init(device, size, size);
	xMinus->init(device, size, size);
	yPlus->init(device, size, size);
	yMinus->init(device, size, size);
	zPlus->init(device, size, size);
	zMinus->init(device, size, size);
}

void KEngine::KComponent::ProbeComponent::initCoefficients()
{
	type = PROBE_TYPE::IRRADIANCE;
	coefficients = std::make_unique<float*>(new float[9]);
}

