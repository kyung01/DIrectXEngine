#pragma once
#include <Engine\Enum.h>
#include <Engine\Componenets\Component.h>

namespace KEngine {
	namespace KComponent {

		struct Renderable : public Component {
		private:
			bool m_isWorldMatrixDirty;
			Vector3 m_position;
			Vector3 m_scale;
			Quaternion m_rotation;
			Matrix m_worldMatirx;

		public:
			KEnum meshId;

			Renderable();
			Vector3		getPosition();
			Vector3		getScale();
			Quaternion	getRotation();
			void setPosition(Vector3 position);
			void setScale(Vector3 position);
			void setRotation(Quaternion quaternion);
			Matrix getWorldMatrix();
		};
	}
}