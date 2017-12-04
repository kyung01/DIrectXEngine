#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\ProbeComponent.h>

using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class ProbeSystem : public System<ProbeComponent> {
		private:
		protected:
			/*
			std::vector<PointLightInfo> m_pointLights;
			std::vector<SpotLightInfo> m_spotLights;
			*/
			void addEntityHandle(Entity& entity, ProbeComponent &componenet) override;
			void addEntityLinkRecreate(std::vector<Entity> & entityVectors, ProbeComponent &componenet) override;
		public:
			ProbeSystem();
			void init();

			void update(std::vector<Entity> &entities, float time) override;
		};
	}

}