#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include <Graphic\Scene\ILight.h>
#include <Graphic\AtlasReference.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class SpotLight : public Camera ,public ILight {
		public:
			SpotLight();


		};
	}
}