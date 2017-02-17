#pragma once
#include "Light.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class PointLight : public Light {

			Matrix 
				m_matLookXPlus, m_matLookXMinus,
				m_matLookYPlus, m_matLookYMinus,
				m_matLookZPlus, m_matLookZMinus;
			void updatePointLightViewMatrixs();
		public:
			PointLight();
			Matrix getViewMatrix() override;
			Matrix getMatrixXPlus();
			Matrix getMatrixYPlus();
			Matrix getMatrixZPlus();
			Matrix getMatrixXMinus();
			Matrix getMatrixYMinus();
			Matrix getMatrixZMinus();
		};
	}
}