#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "directxtex.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

public:
    inline ID3D11Device* GetDevice() const { return mDevice; }
	inline ID3D11DeviceContext* GetDeviceContext() const {return mDeviceContext;}
	inline ID3D11DepthStencilView* GetDepthStencilView() const {return mDepthStencilView;}

	inline void GetProjectionMatrix(XMMATRIX& projectionMatrix) { projectionMatrix = mProjectionMatrix; }
	inline void GetWorldMatrix(XMMATRIX& worldMatrix){worldMatrix = mWorldMatrix;}
	inline void GetOrthoMatrix(XMMATRIX& orthoMatrix){orthoMatrix = mOrthoMatrix;}

	void GetVideoCardInfo(char* cardName, int& memory);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool mVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	XMMATRIX mProjectionMatrix;
	XMMATRIX mWorldMatrix;
	XMMATRIX mOrthoMatrix;

	ID3D11DepthStencilState* mDepthDisabledStencilState;

	ID3D11BlendState* mAlphaEnableBlendingState;
	ID3D11BlendState* mAlphaDisableBlendingState;

	D3D11_VIEWPORT mViewPort;
};

