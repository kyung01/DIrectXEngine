#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

namespace KEngine {
	class EventHandler {
	public:

		//pure data update
		virtual void init(ID3D11Device * device, ID3D11DeviceContext * context, int windowWidth, int windowHeight);
		virtual void update(float timeElapsed);
		virtual void render(
			ID3D11Device * device, ID3D11DeviceContext * context,
			ID3D11RenderTargetView * target, ID3D11DepthStencilView * targetDepth, D3D11_VIEWPORT  viewport);
		virtual void OnMouseMove(WPARAM buttonState, int x, int y);
		virtual void OnResize(int windowWidth, int windowHeight);
	};
}