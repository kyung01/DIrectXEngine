#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
#include <Graphic\Scene\IObject.h>
#include <Graphic\AtlasReference.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class IProbeReflection {
			RenderTexture	m_target;
			DepthTexture	m_depth;
		};
	}
}