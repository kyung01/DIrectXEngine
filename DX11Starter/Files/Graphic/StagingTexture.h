#pragma once

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>

namespace NGraphic {

	////////////////////////////////////////////////////////////////////////////////
	// Class name: RenderTextureClass
	////////////////////////////////////////////////////////////////////////////////
	class StagingTexture
	{
	private:
		bool m_isInitialized;
		int m_width, m_height;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11Texture2D* m_renderTargetTexture;
	public:
		StagingTexture();
		~StagingTexture();

		bool init(ID3D11Device*, DXGI_FORMAT format, int, int);
		void release();

		ID3D11Texture2D* getShaderResource();
		ID3D11ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView * getRenderTargetView();

		int getWidth();
		int getHeight();
	};


}