#pragma once
#include <memory>
#include <map> 
#include <vector> 
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Componenets\Component.h>
using namespace DirectX::SimpleMath;

namespace KEngine {
	namespace KComponent {

		struct Transform3D :Component{
			bool isDirty;
			Vector3 position;
			Vector3 size;
			Quaternion rotation;
			Transform3D();
			void setPosition(float x, float y, float z);
			void setPosition(Vector3 position);
			void setScale(Vector3 position);
			void setRotation(Quaternion quaternion);

		};
	}
}