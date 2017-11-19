#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector> 
#include <Engine\Systems\System.h>
#include <Engine\Componenets\Transform3D.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
using namespace KEngine::KComponent;
namespace KEngine {
	namespace KSystem {
		class Transform3DSystem : public System<Transform3D> {
		private:
		protected:
			//helper methods
			void addEntityHandle(Entity& entity, Transform3D &componenet) override;
			void addEntityLinkRecreate(std::vector<Entity> & entityVectors, Transform3D &componenet) override;
		public:
			 void update(std::vector<Entity> &entities, float timeElapsed) override;
		};
	}

}