#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Camera.h>
#include <Engine\Systems\System.h>
#include <Engine\Componenets\LightEntity.h>
#include <Engine\SimpleShader.h>
#include <Engine\Mesh.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class LightSystem : public System<LightEntity> {
		private:
		protected:
			std::vector<PointLightInfo> m_pointLights;
			std::vector<SpotLightInfo> m_spotLights;
			void addEntityHandle(Entity& entity, LightEntity &componenet) override;
		public:
			void setPointLight(int index,  Vector3 color, float intensity);
			void setSpotLight (int index, Vector3 color, float intensity, float cornRadius);
		};
	}

}