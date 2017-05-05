#include "Graphic\StagingTexture.h"
using namespace NGraphic;
StagingTexture::StagingTexture()
{
	m_isInitialized = false;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
	m_renderTargetTexture = 0;
}
StagingTexture::~StagingTexture()
{
	release();
}

bool StagingTexture::init(ID3D11Device* device, int textureWidth, int textureHeight)
{
	release();
	m_isInitialized = true;
	m_width = textureWidth;
	m_height = textureHeight;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;



	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.BindFlags = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;


	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(result))
	{
		m_renderTargetTexture->Release();
		return false;
	}
	/*
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

	*/
	//m_renderTargetTexture->Release();

	return true;
}
ID3D11Texture2D * StagingTexture::getShaderResource()
{
	return m_renderTargetTexture;
}

int NGraphic::StagingTexture::getWidth()
{
	return m_width;
}

int NGraphic::StagingTexture::getHeight()
{
	return m_height;
}

void StagingTexture::release()
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