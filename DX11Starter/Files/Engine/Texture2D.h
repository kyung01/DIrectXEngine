#pragma once
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>

//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;
namespace KEngine {

	struct Texture2D {
	private:
		bool m_isAlive;
		bool m_isInitialized;
		int m_width, m_height;
		D3D11_VIEWPORT viewport;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11Texture2D* m_renderTargetTexture;
		void init();
	public:
		Texture2D();
		Texture2D(const Texture2D&);
		~Texture2D();

		bool init(ID3D11Device*, int, int);
		bool initCube(ID3D11Device*, int, int, void* data);
		bool initCubeArray(ID3D11Device*, int, int, int, void* data);
		void release();

		void setRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
		void clear(ID3D11DeviceContext*, float, float, float, float);
		ID3D11Texture2D* getShaderResource();
		ID3D11ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView * getRenderTargetView();

		int getWidth();
		int getHeight();
		void setViewport(D3D11_VIEWPORT viewport);
		void setRenderTargetView(ID3D11RenderTargetView* view, D3D11_VIEWPORT& viewport);
		D3D11_VIEWPORT getViewport();

	};

}