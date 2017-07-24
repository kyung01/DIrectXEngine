#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Systems\SystemFlawed.h>
#include <Engine\Componenets\SphericalHarmonicProbe.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {

		class LightRender : SystemFlawed {
			std::vector<SphericalHarmonicProbe> m_components;
		public:

			void addEntity(Entity& entity) override;
			void update(float time) override;
			void render(ID3D11Device * device, ID3D11DeviceContext * context);
		};
	}

}