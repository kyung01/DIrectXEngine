#pragma once
#include "Camera.h"
#include <SimpleMath.h>
#include "Graphic\RenderTexture.h"
#include "Graphic\DepthTexture.h"
#include <Graphic\Scene\Camera.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class SphericalCamera : public Camera {
		protected:
			Matrix
				m_matLookXPlus, m_matLookXMinus,
				m_matLookYPlus, m_matLookYMinus,
				m_matLookZPlus, m_matLookZMinus;
		public:
			SphericalCamera();
			virtual void updatePointLightViewMatrixs();
			Matrix getViewMatrix() override;
			Matrix getMatrixXPlus();
			Matrix getMatrixYPlus();
			Matrix getMatrixZPlus();
			Matrix getMatrixXMinus();
			Matrix getMatrixYMinus();
			Matrix getMatrixZMinus();
		};

		class OldSphericalCamera : public OldCamera {
		protected:
			Matrix
				m_matLookXPlus, m_matLookXMinus,
				m_matLookYPlus, m_matLookYMinus,
				m_matLookZPlus, m_matLookZMinus;
		public:
			OldSphericalCamera();
			virtual void updatePointLightViewMatrixs() ;
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