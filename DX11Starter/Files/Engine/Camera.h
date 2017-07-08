#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
namespace KEngine {
	struct Camera {
	protected:
		Vector3
			position;
		Quaternion rotation;
		//projection matrix data
		float
			m_fov,
			m_screenWidth, m_screenHeight,
			m_clipNear, m_clipFar;
		Matrix
			//worldMatirx, 
			viewMatrix, 
			projMatrix;
		bool
			//isDirty_world,
			m_isDirtyView,

			//isDirty_worldView,
			//isDirty_matView,
			m_isDirtyProj;
	public:
		Camera();
		void setProjParameters(float fov, float screenWidth, float screenHeight, float clipNear, float clipFar);
		Vector3 getPosition();
		Vector3 getLookDir();
		Matrix 
			getViewMatrix(),
			//getViewMatrix(),
			getProjMatrix();
		void setQuaternion(Quaternion rotation);
		void setPosition(Vector3 position);
	};
}