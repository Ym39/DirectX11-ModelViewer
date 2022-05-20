#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "Material.h"
#include "LightManager.h"

using namespace DirectX;
using namespace std;

class LightMeshShader
{
private:
	struct PerObejct
	{
		XMMATRIX modelViewProjection;
		XMMATRIX modelView;
	};

	struct MaterialBuffer
	{
		ShaderUploadMaterial mat;
	};

	struct LightBuffer
	{
		XMFLOAT4 positionWS;
		XMFLOAT4 directionWS;
		XMFLOAT4 positionVS;
		XMFLOAT4 directionVS;
		XMFLOAT4 color;
		float spotlightAngle;
		float range;
		float intensity;
		bool enabled;
		bool selected;
		int type;
		XMFLOAT2 padding;
	};

public:
	LightMeshShader();
	~LightMeshShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Material* material);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, Material* material);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;

	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mMaterialBuffer;
	ID3D11Buffer* mLightBuffer;
	ID3D11ShaderResourceView* mSrv;

	ID3D11SamplerState* mSampleStateWarp;
	ID3D11SamplerState* mSampleStateClamp;
};

