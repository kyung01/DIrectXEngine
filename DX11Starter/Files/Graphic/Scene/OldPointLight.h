#pragma once
#include <Graphic\Scene\ILight.h>
#include <Graphic\Scene\OldSpotLight.h>
#include <Graphic\Scene\SphericalCamera.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class OldPointLight : public OldSpotLight {

		protected:
			Matrix
				m_matLookXPlus, m_matLookXMinus,
				m_matLookYPlus, m_matLookYMinus,
				m_matLookZPlus, m_matLookZMinus;
		public:
			OldPointLight();
			virtual void updatePointLightViewMatrixs();
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