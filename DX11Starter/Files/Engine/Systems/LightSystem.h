#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\LightEntity.h>
#include <ClusteredFrustum\Frustum.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class LightSystem : public System<LightComponent> {
		private:
			static const float RADIUS_PER_LIGHT_INTENSITY;
		protected:
			KFrustum::Frustum m_frustum;
			std::vector<PointLightInfo> m_pointLights;
			std::vector<SpotLightInfo> m_spotLights;
			void addEntityHandle(Entity& entity, LightComponent &componenet) override;
			void addEntityLinkRecreate(Entity& entity, LightComponent &componenet) override;
		public:
			void setPointLight(int index,  Vector3 color, float intensity);
			void setSpotLight (int index, Vector3 color, float intensity, float cornRadius);
			void run();
		};
	}

}