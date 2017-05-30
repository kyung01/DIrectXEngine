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

		struct SphericalHarmonicProbe {
			Vector3 position;
			//float dummy0;

			float 
				c1, c2, c3, c4, 
				c5, c6, c7, c8, 
				c9;
			//float dummy1, dummy1, dummy2;



		};
	}
}