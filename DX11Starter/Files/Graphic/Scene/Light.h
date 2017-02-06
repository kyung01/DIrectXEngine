#pragma once
#include "Camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Light : public Camera {
		protected:
		public:
			Light();
			LIGHT_TYPE m_lightType;
			Vector4 m_lightColor;
			float m_lightDistance;
			float m_lightAngle;

		};
	}
}