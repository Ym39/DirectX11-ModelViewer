#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
using namespace DirectX;

class GridClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	GridClass();
	~GridClass();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount() const { return mIndexCount; }

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;
};

