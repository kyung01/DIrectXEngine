#pragma once
#include "Object.h"
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Camera : public Object {
		protected:
			float
				m_fov,
				m_screenWidth, m_screenHeight,
				m_clipNear, m_clipFar;
			Matrix	m_matProjection,
				m_matView;
		protected:
		public:
			bool	m_isDirty_matProjection,
					m_isDirty_matView;
			CAMERA_TYPE viewType;
			//mat4 position, rotation;
			//vec3 up;

			float zNear, zFar;

			Camera();
			float getFOV();
			Matrix getProjectionMatrix();
			Matrix getProjectionMatrix(float width, float height);
			Matrix getProjectionMatrix(float fov,float screen_width, float screen_height, float cam_near, float cam_far);
			virtual Matrix getViewMatrix();
			void setFOV(float value);
			Object setPos(Vector3 pos) override;
			Object setRotation(Quaternion quaternion) override ;
			
			
			//mat4 getProjViewMatrix();
		};
	}
}