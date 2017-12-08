#include <Engine\Componenets\ReflectionComponent.h>
using namespace KEngine::KComponent;

ReflectionComponent::ReflectionComponent() :
	isNeedUpdate(true)
{
	initCoefficients();
}

void KEngine::KComponent::ReflectionComponent::initRenderTexture(ID3D11Device *device, int size)
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

void KEngine::KComponent::ReflectionComponent::initCoefficients()
{
	type = PROBE_TYPE::IRRADIANCE;
}

