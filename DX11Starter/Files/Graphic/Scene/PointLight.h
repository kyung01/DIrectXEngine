#pragma once
#include "Light.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class PointLight : public Light {

			Matrix 
				m_lookXPlus, m_lookXMinus,
				m_lookYPlus, m_lookYMinus,
				m_lookZPlus, m_lookZMinus;
			void updatePointLightViewMatrixs();
		public:
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