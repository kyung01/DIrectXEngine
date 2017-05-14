#pragma once
#include <Graphic\Scene\SpotLight.h>
#include <SimpleMath.h>
#include <Graphic\Scene\SphericalCamera.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class PointLight : public ILight, public SphericalCamera {

		public:
			PointLight();
		};
	}
}