#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		enum LightType{PointLight, SpotLight };
		/*
		this struct should be paired with LightRender system
		*/
		struct LightEntity : public Component {
		private:
		public:
			LightType lightType;
			int index;
		};
		struct PointLightInfo {
			//Vector3 position;
			Vector3 color;
			float intensity;
		};
		struct SpotLightInfo {
			//Vector3 position;
			//Quaternion rotation;
			Vector3 color;
			float intensity;
			float cornRadius;
		};

	}
}