#include "RenderTextureClass.h"

RenderTextureClass* gDepthTexture;

RenderTextureClass::RenderTextureClass():
	m_renderTargetTexture(nullptr),
	m_renderTargetView(nullptr),
	m_shaderResourceView(nullptr)
{
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass&)
{
}

RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device* device, int textureWidth, int textureHeight,float screenDepth, float screenNear)
{
	mTextureWidth = textureWidth;
	mTextureHeight = textureHeight;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

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

	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(result))
		return false;

	//렌더 타겟 뷰 디스크립션 설정
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
		return false;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
		return false;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰를 만듭니다.
	result = device->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// 렌더링을 위해 뷰포트를 설정합니다.
	mViewPort.Width = (float)textureWidth;
	mViewPort.Height = (float)textureHeight;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;

	// 투영 행렬을 설정합니다.
	mProjectionMatrix = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

	// 2D 렌더링을 위한 직교 투영 행렬을 만듭니다.
	mOrthoMatrix = XMMatrixOrthographicLH((float)textureWidth, (float)textureHeight, screenNear, screenDepth);

	return true;
}

void RenderTextureClass::Shutdown()
{
	if (m_shaderResourceView)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	return;
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, mDepthStencilView);

	deviceContext->RSSetViewports(1, &mViewPort);
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return m_shaderResourceView;
}

void RenderTextureClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = mProjectionMatrix;
}

void RenderTextureClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = mOrthoMatrix;
}
