#pragma once
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>
#include <Graphic\RenderTexture.h>
#include <Graphic\DepthTexture.h>
#include <Graphic\AtlasReference.h>
using namespace DirectX::SimpleMath;

namespace KEngine {
	namespace KComponent {

		struct ForwardLight {
			Vector3 position;
			NGraphic::AtlasReference atlasReference;
			//NGraphic::RenderTexture m_target;
			//NGraphic::DepthTexture	m_depth;



		};
	}
}