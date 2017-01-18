#include "Graphic\RenderTexture.h"



using namespace NGraphic;

void NGraphic::RenderTexture::setViewport(int textureWidth, int textureHeight)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)textureWidth;
	viewport.Height = (float)textureHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

RenderTexture::RenderTexture()
{
	m_isInitialized = false;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}


RenderTexture::RenderTexture(const RenderTexture& other)
{
	m_isInitialized = false;
}


RenderTexture::~RenderTexture()
{
	release();
}


bool NGraphic::RenderTexture::init(ID3D11Device *device, IDXGISwapChain * swapChain, int textureWidth, int textureHeight)
{
	release();
	m_isInitialized = true;
	HRESULT result;
	ID3D11Texture2D* m_renderTargetTexture;
	setViewport(textureWidth, textureHeight);

	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_renderTargetTexture));
	if (FAILED(result))
	{
		return false;
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}
	//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	//// Setup the description of the shader resource view.
	//shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	//shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	//result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	//
	//if (FAILED(result))
	//{
	//	m_renderTargetTexture->Release();
	//	return false;
	//}
	m_renderTargetTexture->Release();
	return true;
}

bool RenderTexture::init(ID3D11Device* device, int textureWidth, int textureHeight)
{
	release();
	m_isInitialized = true;
	m_width = textureWidth;
	m_height = textureHeight;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ID3D11Texture2D* m_renderTargetTexture;
	setViewport(textureWidth, textureHeight);


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width					= textureWidth;
	textureDesc.Height					= textureHeight;
	textureDesc.MipLevels				= 1;
	textureDesc.ArraySize				= 1;
	textureDesc.Format					= DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count		= 1;
	textureDesc.Usage					= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags				= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags			= 0;
	textureDesc.MiscFlags				= 0;


	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}
	m_renderTargetTexture->Release();

	return true;
}


void RenderTexture::release()
{
	if (!m_isInitialized)
		return;
	if (m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

}


void RenderTexture::setRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);

	return;
}


void RenderTexture::clear(ID3D11DeviceContext* deviceContext,
	float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.

	return;
}

ID3D11RenderTargetView * RenderTexture::getRenderTargetView() {
	return m_renderTargetView;
}

int NGraphic::RenderTexture::getWidth()
{
	return m_width;
}

int NGraphic::RenderTexture::getHeight()
{
	return m_height;
}

void NGraphic::RenderTexture::setRenderTargetView(ID3D11RenderTargetView * view, D3D11_VIEWPORT &viewport)
{
	m_renderTargetView = view;
	this->viewport = viewport;
}

D3D11_VIEWPORT NGraphic::RenderTexture::getViewPort()
{
	return viewport;
}

ID3D11ShaderResourceView* RenderTexture::getShaderResourceView()
{
	return m_shaderResourceView;
}