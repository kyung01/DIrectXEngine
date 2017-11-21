#pragma once
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\AtlasComponent.h>

using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class AtlasSystem : public System<AtlasComponent> {
		private:
			static const float RADIUS_PER_LIGHT_INTENSITY;
			float m_width, m_height;
			float m_sliceWidth, m_sliceHeight;
			float xBegin, yBegin;
		protected:
			void addEntityHandle(Entity& entity, AtlasComponent &componenet) override;
			void addEntityLinkRecreate(std::vector<Entity> & entityVectors, AtlasComponent &componenet) override;
		public:
			AtlasSystem();
			void init(int width, int height, int sliceWidth, int sliceHeight);
			void update(std::vector<Entity> &entities, float time) override;
		};
	}

}