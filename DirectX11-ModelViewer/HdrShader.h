#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Common.h"
#include "AssetClass.h"

class HdrShader
{
public:
	HdrShader();
	~HdrShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, WideMaterial* material);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, WideMaterial* material);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;

	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mMaterialBuffer;

	ID3D11SamplerState* mSampleStateWarp;
};

