#pragma once
#include <d3d11.h>
#include "Utility.h"

class MeshClass
{
public:
	MeshClass();
    
	MeshClass& operator=(const SkinnedMeshData& meshData);

	bool Initalize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);
	void Shutdown();
    
	unsigned int GetIndexCount() const { return indices.size(); }
	bool IsInitalized() const { return mIsInitialize; }
private:
	bool mIsInitialize = false;

	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	vector<InputVertex> vertices;
	vector<unsigned int> indices;
	vector<Bone> bones;
};

