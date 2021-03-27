#pragma once
#include<d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int,float,float);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView();
	void GetProjectionMatrix(XMMATRIX& projectionMatrix);
	void GetOrthoMatrix(XMMATRIX& orthoMatrix);

private:
	int mTextureWidth;
	int mTextureHeight;

	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewPort;
	XMMATRIX mProjectionMatrix;
	XMMATRIX mOrthoMatrix;
};

