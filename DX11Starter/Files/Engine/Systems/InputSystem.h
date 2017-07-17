#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <Engine\Systems\System.h>
#include <Engine\Camera.h>

using namespace DirectX::SimpleMath;
namespace KEngine {
	namespace KSystem {
		class InputSystem : System {
			Vector3 m_playerPosition;
			Quaternion m_playerRotation;
		protected:

			int mouseMoveXY[2] = { -1,-1 };
			float mouseRotation[2] = { 0,0 };
		public:
			Vector3 getPosition();
			Quaternion getRotation();
			/*
			update WASD moevment
			*/
			void update(float time) override;
			/*
			update rotation
			*/
			void OnMouseMove(WPARAM buttonState, int x, int y);
			
		};
	}
}