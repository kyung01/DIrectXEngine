#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {
		/*
		This struct is used as part of Forward+ cluster frustum rendering process.
		This struct contains information about the light for the system to test the light.
		The system should be able to tell number of lights continaed by the virtual block space by iterating through array of struct
		*/
		struct ClusterPointLight : public Component {
		private:

		public:
			Vector3 position;
			float radius;
		};
		struct ClusterSpotLight : public ClusterPointLight {
		private:
		public:
			Quaternion rotation;
		};
	}
}