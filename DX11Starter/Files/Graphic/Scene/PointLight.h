#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include <Graphic\Scene\ILight.h>
#include <Graphic\AtlasReference.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
#include <Graphic\Scene\SphericalCamera.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class PointLight : public ILight, public SphericalCamera {
		public:

			RenderTexture	m_textureTarget;
			DepthTexture	m_textureDepth;


			PointLight();


		};
	}
}