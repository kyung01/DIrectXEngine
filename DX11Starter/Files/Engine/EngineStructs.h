#pragma once
#include <SimpleMath.h>
#include <DirectX\RenderTexture.h>

namespace KEngine {

	struct ProbeReflection {
		bool isUpdated;
		DirectX::SimpleMath::Vector3 position;
		NDirectX::RenderTexture xPlus, xMius, yPlus, yMinus, zPlus, zMinus;
		ProbeReflection(int textureSize);
	};
	struct ProbeIrradiance {
		DirectX::SimpleMath::Vector3 position;
		float c1, c2, c3, c4, c5, c6, c7, c8, c9;

	};
}