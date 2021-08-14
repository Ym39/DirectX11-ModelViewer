#pragma once
#include"Bounds.h"
#include<d3d11.h>
using namespace DirectX;

class BoundModel
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	};

public:
	BoundModel() = delete;
	BoundModel(ID3D11Device* device,const Bounds& bounds);
	~BoundModel();
	
	bool Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount() const { return mIndexCount; }

private:
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;
};

