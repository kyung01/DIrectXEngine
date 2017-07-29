#pragma once
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Entity.h>

namespace DirectX {
	struct DirectXSimpleMath {

		static DirectX::SimpleMath::Vector3 GetCombinedMax(DirectX::SimpleMath::Vector3& a, DirectX::SimpleMath::Vector3& b);
		static DirectX::SimpleMath::Vector3 GetCombinedMin(DirectX::SimpleMath::Vector3& a, DirectX::SimpleMath::Vector3& b);
	};
}