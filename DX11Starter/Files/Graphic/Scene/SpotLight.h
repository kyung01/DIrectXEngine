#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include <Graphic\AtlasReference.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class SpotLight : public Camera {
			bool m_isLightDirty;

			Vector3 m_lightColor;
		public:
			SpotLight();
			LIGHT_TYPE m_lightType;
			float m_lightDistance;

			std::shared_ptr<RenderTexture>	m_deferredTexture;
			std::shared_ptr<DepthTexture>	m_deferredDepth;

			AtlasReference m_atlas;


			void setLightColor(Vector3 color);
			void setLightColor(float r, float g, float b);
			void setLightType();

			Vector3 getLightColor();
		};
	}
}