#pragma once
#include <memory>
#include <map> //TODO delete
#include <vector> //TODO delete
#include <d3d11.h>
#include <SimpleMath.h>

#include <Engine\Entity.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
namespace KEngine {
	namespace KSystem {

		//need to be fixed
		class SystemFlawed {

		protected:
		public:
			virtual void addEntity(Entity& entity);
			virtual void update(float time);
			//virtual void render(ID3D11Device * device, ID3D11DeviceContext * context);

		};
	}

}