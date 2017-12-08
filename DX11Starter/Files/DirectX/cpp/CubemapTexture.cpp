#include <DirectX\CubemapTexture.h>

#include <DirectX\DirectXUtility.h>

using namespace NDirectX;


CubemapTexture::CubemapTexture()
{
	m_isInitialized = false;
	m_renderTargetViews = 0;
	m_shaderResourceView = 0;
}


CubemapTexture::CubemapTexture(const CubemapTexture& other)
{
	m_isInitialized = false;
	m_renderTargetViews = 0;
	m_shaderResourceView = 0;
}


CubemapTexture::~CubemapTexture()
{
	release();
}
void CubemapTexture::init() {

	m_isInitialized = false;
	m_renderTargetViews = new ID3D11RenderTargetView*[6];
	m_shaderResourceView = 0;
}


//https://stackoverflow.com/questions/38752215/directx-rendering-to-texture2darray-and-loading-from-it-in-compute-shader
bool CubemapTexture::initCube(ID3D11Device* device, int textureWidth, int textureHeight, void * data)
{
	release();
	m_isInitialized = true;
	m_width = textureWidth;
	m_height = textureHeight;


	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = textureWidth;
	viewport.Height = textureHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


	// Create the render target texture.
	if (data == 0) {

		result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	}
	else {

		result = device->CreateTexture2D(&textureDesc, (const D3D11_SUBRESOURCE_DATA *)data, &m_renderTargetTexture);
	}
	DirectX::DirectXUtility::HRESULT_CHECK(result);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	DirectX::DirectXUtility::HRESULT_CHECK(result);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}
	//m_renderTargetTexture->Release();

	return true;
}


void CubemapTexture::release()
{
	if (!m_isInitialized)
		return;

	if (m_renderTargetTexture) {
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}
	if (m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = 0;
	}

	if (m_renderTargetViews)
	{
		for (int i = 0; i < 6; i++) {
			m_renderTargetViews[i]->Release();
			m_renderTargetViews[i] = 0;
		}
		delete m_renderTargetViews;
	}

}




ID3D11Texture2D * CubemapTexture::getShaderResource()
{
	return m_renderTargetTexture;
}

ID3D11ShaderResourceView* CubemapTexture::getShaderResourceView()
{
	return m_shaderResourceView;
}
ID3D11RenderTargetView *& NDirectX::CubemapTexture::getRenderTargetView(int index)
{
	// TODO: insert return statement here
	return m_renderTargetViews[index];
}

int CubemapTexture::getWidth()
{
	return m_width;
}

int CubemapTexture::getHeight()
{
	return m_height;
}

D3D11_VIEWPORT CubemapTexture::getViewport()
{
	return viewport;
}
