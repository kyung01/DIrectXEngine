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
	cubemap = std::make_unique<NDirectX::RenderTexture>();
	cubemap->initCube(device, size, size);
}

void KEngine::KComponent::ReflectionComponent::initCoefficients()
{
	type = PROBE_TYPE::IRRADIANCE;
}

