#pragma once

#include <d3d11.h>
namespace NGraphic {
	class RenderStateStack {
		D3D11_VIEWPORT viewport;
	public:
		void save(ID3D11DeviceContext *context);
		void load(ID3D11DeviceContext *context);
		
	};
}