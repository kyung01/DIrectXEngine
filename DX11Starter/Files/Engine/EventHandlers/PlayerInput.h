#pragma once
#include <Engine\EventHandler.h>
namespace KEngine {
	namespace KHandler {

		class PlayerInput : public EventHandler {
			DirectX::SimpleMath::Vector3 m_playerPosition;
			DirectX::SimpleMath::Quaternion m_playerRotation;
		protected:

			int mouseMoveXY[2] = { -1,-1 };
			float mouseRotation[2] = { 0,0 };
		public:
			DirectX::SimpleMath::Vector3 getPosition();
			DirectX::SimpleMath::Quaternion getRotation();

			void init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight) override; 
			void update(float timeElapsed) override;
			void render(
				ID3D11Device * device, ID3D11DeviceContext * context,
				ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport);
			void OnMouseMove(WPARAM buttonState, int x, int y) override;
			void OnResize(int windowWidth, int windowHeight) override;
		};
	}
}