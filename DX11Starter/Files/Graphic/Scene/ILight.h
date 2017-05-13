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
		class ILight  {
		protected:
			bool m_isLightDirty;
			Vector3 m_lightColor;
		public:
			ILight();
			LIGHT_TYPE m_lightType;
			float m_lightDistance;

			std::shared_ptr<RenderTexture>	m_deferredTexture;
			std::shared_ptr<DepthTexture>	m_deferredDepth;

			AtlasReference m_atlas;


			void setLightColor(Vector3 color);
			void setLightColor(float r, float g, float b);
			Vector3 getLightColor();
		};
	}
}