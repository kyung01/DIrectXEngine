#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		enum LIGHT_TYPE{POINT_LIGHT, SPOT_LIGHT };
		/*
		this struct should be paired with LightRender system
		*/
		struct LightComponent : public Component {
		private:
		public:
			LIGHT_TYPE lightType;
			int lightIndex;
		};
		struct PointLightInfo {
			Vector3 position;
			Vector3 color;
			float intensity;
		};
		struct SpotLightInfo {
			Vector3 position;
			Quaternion rotation;
			Vector3 color;
			float intensity;
			float fov;
		};

	}
}