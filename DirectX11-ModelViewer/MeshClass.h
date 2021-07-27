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
	bool IsSkinning() const { return mIsSkinning; }

	vector<Bone>& GetBones() { return bones; }
	Bone& GetBone(int index) { return bones[index]; }
private:
	bool mIsInitialize = false;
	bool mIsSkinning = false;

	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	vector<InputVertex> vertices;
	vector<unsigned int> indices;
	vector<Bone> bones;
};

