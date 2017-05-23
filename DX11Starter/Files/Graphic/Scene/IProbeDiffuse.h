#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
#include <Graphic\Scene\IObject.h>
#include <Graphic\AtlasReference.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class IProbeDiffuse {
			
		public:
			IProbeDiffuse();
			//The nine coefficients 
			std::vector<Vector3> m_coefficients; 

		};
	}
}