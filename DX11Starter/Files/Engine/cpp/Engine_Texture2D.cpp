#include <Engine\Texture2D.h>
#include <DirectX\DirectXUtility.h>
using namespace KEngine;


Texture2D::Texture2D()
{
	m_isInitialized = false;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}


Texture2D::Texture2D(const Texture2D& other)
{
	m_isInitialized = false;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}


Texture2D::~Texture2D()
{
	release();
}
void Texture2D::init() {

	m_isInitialized = false;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}

bool Texture2D::init(ID3D11Device* device, int textureWidth, int textureHeight)
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
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


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
	//m_renderTargetTexture->Release();

	return true;
}

bool Texture2D::initCube(ID3D11Device* device, int textureWidth, int textureHeight, void * data)
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
	result = device->CreateTexture2D(&textureDesc, (const D3D11_SUBRESOURCE_DATA *)data, &m_renderTargetTexture);
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

bool Texture2D::initCubeArray(ID3D11Device* device, int textureWidth, int textureHeight, int arraySize, void * data)
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
	textureDesc.ArraySize = arraySize * 6;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, (const D3D11_SUBRESOURCE_DATA *)data, &m_renderTargetTexture);
	DirectX::DirectXUtility::HRESULT_CHECK(result);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
	shaderResourceViewDesc.TextureCubeArray.MostDetailedMip = 0;
	shaderResourceViewDesc.TextureCubeArray.MipLevels = 1;
	shaderResourceViewDesc.TextureCubeArray.First2DArrayFace = 0;
	shaderResourceViewDesc.TextureCubeArray.NumCubes = arraySize;

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


void Texture2D::release()
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

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

}


void Texture2D::setRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);

	return;
}


void Texture2D::clear(ID3D11DeviceContext* deviceContext,
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

ID3D11Texture2D * Texture2D::getShaderResource()
{
	return m_renderTargetTexture;
}

ID3D11RenderTargetView * Texture2D::getRenderTargetView() {
	return m_renderTargetView;
}

int Texture2D::getWidth()
{
	return m_width;
}

int Texture2D::getHeight()
{
	return m_height;
}

void Texture2D::setViewport(D3D11_VIEWPORT viewport)
{
	this->viewport = viewport;
}

void Texture2D::setRenderTargetView(ID3D11RenderTargetView * view, D3D11_VIEWPORT &viewport)
{
	m_renderTargetView = view;
	this->viewport = viewport;
}

D3D11_VIEWPORT Texture2D::getViewport()
{
	return viewport;
}

ID3D11ShaderResourceView* Texture2D::getShaderResourceView()
{
	return m_shaderResourceView;
}