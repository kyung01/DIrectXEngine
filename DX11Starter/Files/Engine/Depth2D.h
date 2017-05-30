#pragma once
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
namespace KEngine {

	struct Depth2D {
	private:
		bool m_isInitialzed;
		float m_width, m_height;
		ID3D11DepthStencilView *m_depthView = 0;
		ID3D11ShaderResourceView *m_shaderResourceView = 0;
	public:
		float getWidth();
		float getHeight();
		Depth2D();
		~Depth2D();
		bool init(ID3D11Device* device, int textureWidth, int textureHeight);
		void setDepthStencilView(ID3D11DepthStencilView * depth);
		void clear(ID3D11DeviceContext *context);
		void release();

		ID3D11DepthStencilView * getDepthStencilView();
		ID3D11ShaderResourceView* getShaderResourceView();

	};

}