#pragma once
#include "Camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Light : public Camera {
			bool m_isLightDirty;

			Vector3 m_lightColor;
		public:
			Light();
			LIGHT_TYPE m_lightType;
			float m_lightDistance;

			void setLightColor(Vector3 color);
			void setLightColor(float r, float g, float b);
			void setLightType();

			Vector3 getLightColor();
		};
	}
}