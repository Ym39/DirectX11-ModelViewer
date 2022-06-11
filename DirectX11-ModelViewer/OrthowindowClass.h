#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class OrthowindowClass
{
public:
	OrthowindowClass();
	~OrthowindowClass();

	bool Initialize(ID3D11Device* device, int windowWidth, int windowHeight);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount() const;
private:
	bool InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
};

