#pragma once
#include <d3d11.h>
#include "Utility.h"

class SubMesh
{
public:
	SubMesh(const SaveSubMesh& subMesh);

	bool Initalize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);
	void Shutdown();

	unsigned int GetIndexCount() const { return indices.size(); }
	const vector<InputVertex>& GetVertices() const { return vertices; }
private:
	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	vector<InputVertex> vertices;
	vector<unsigned int> indices;
};

