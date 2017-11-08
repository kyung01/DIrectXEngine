#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\LightEntity.h>
#include <ClusteredFrustum\Frustum.h>

using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class LightSystem : public System<LightComponent> {
		private:
			static const float RADIUS_PER_LIGHT_INTENSITY;
		protected:
			bool m_isFrustumNeedUpdate; 
			KFrustum::Frustum m_frustum;
			std::vector<PointLightInfo> m_pointLights;
			std::vector<SpotLightInfo> m_spotLights;
			void addEntityHandle(Entity& entity, LightComponent &componenet) override;
			void addEntityLinkRecreate(Entity& entity, LightComponent &componenet) override;
		public:
			LightSystem();
			void init(float widthOverHeight, float nearDistance, float farDistance, int divisionX, int divisionY, int divisionZ);

			void update(std::vector<Entity> &entities, float time) override;
			void setPointLight(int index,  Vector3 color, float intensity);
			void setSpotLight (int index, Vector3 color, float intensity, float cornRadius);
			int				getLightCount();
			LIGHT_TYPE		getLightType(int n);
			PointLightInfo	getPointLight(int n);
			SpotLightInfo	getSpotLight(int n);
			void run();
		};
	}

}