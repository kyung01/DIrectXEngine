#pragma once
#include <memory>
#include <map> 
#include <vector> 
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace KEngine {
	namespace KComponent {

		struct Transform3D {
			Vector3 position;
			Vector3 size;
			Quaternion rotation;

		};
	}
}