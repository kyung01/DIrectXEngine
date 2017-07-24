#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		enum LightEntityType{PointLight, SpotLight };
		/*
		this struct should be paired with LightRender system
		*/
		struct LightEntity : public Component {
		private:
		public:
			LightEntityType lightType;
			int index;
		};

	}
}