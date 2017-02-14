#pragma once
#include "Camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Light : public Camera {
			bool m_isLightDirty;
		
		public:
			Light();
			LIGHT_TYPE m_lightType;
			Vector3 m_lightColor;
			float m_lightDistance;

			void setLightColor(Vector3 color);
			void setLightColor(float x, float y, float z, float w);
			void setLightType();
		};
	}
}