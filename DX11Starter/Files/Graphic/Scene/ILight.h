#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
#include <Graphic\Scene\IObject.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class ILight  {
		protected:
			bool m_isLightDirty;
			Vector3 m_lightColor;
		public:
			LIGHT_TYPE m_lightType;
			float m_lightDistance;

			//this is for forward rendering
			float
				m_atlasTopLeftX, m_atlasTopLeftY,
				m_atlasViewportWidth, m_atlasViewportHeight;


			void setLightColor(Vector3 color);
			void setLightColor(float r, float g, float b);
		};
	}
}