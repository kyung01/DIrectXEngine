#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\ReflectionComponent.h>

using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class ReflectionSystem : public System<ReflectionComponent> {
		private:
		protected:
			/*
			std::vector<PointLightInfo> m_pointLights;
			std::vector<SpotLightInfo> m_spotLights;
			*/
			void addEntityHandle(Entity& entity, ReflectionComponent &componenet) override;
			void addEntityLinkRecreate(std::vector<Entity> & entityVectors, ReflectionComponent &componenet) override;
		public:
			ReflectionSystem();
			void init();

			void update(std::vector<Entity> &entities, float time) override;
		};
	}

}