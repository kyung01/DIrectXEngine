#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		struct AtlasComponent : public Component {
		private:
		public:
			float x;
			float y;
			float width;
			float height;
		};

	}
}