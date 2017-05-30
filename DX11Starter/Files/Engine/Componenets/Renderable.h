#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {

		struct Renderable : public Component {
			KEnum meshId;
			Vector3 position;
			Vector3 size;
			Quaternion rotation;

		};
	}
}