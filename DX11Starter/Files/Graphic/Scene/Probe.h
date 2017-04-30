#pragma once
#include "PointLight.h"
#include <SimpleMath.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Probe : public PointLight {
		public:
			Probe();
			std::shared_ptr<RenderTexture>	m_deferredTexture;
			std::shared_ptr<DepthTexture>	m_deferredDepth;
			float m_influenceDistance;

			//this is for forward rendering
			//float
			//	m_atlasTopLeftX, m_atlastopLeftY,
			//	m_atlasViewportWidth, m_atlasViewportHeight;


		};
	}
}