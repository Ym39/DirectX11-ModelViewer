#pragma once
#include <d3d11.h>
#include "Utility.h"

class TempMesh
{
public:
	TempMesh();
	~TempMesh() = default;

	void SetMesh(const SkinnedMeshData& data);

	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	bool InitializeBuffer(ID3D11Device* device);

	unsigned int GetIndexCount() const{ return indices.size(); } 

private:
	void ShutdownBuffers();

private:
	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;

	vector<InputVertex> vertices;
	vector<unsigned int> indices;
};

