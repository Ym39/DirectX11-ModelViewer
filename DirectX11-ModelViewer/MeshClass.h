#pragma once
#include <d3d11.h>
#include "Utility.h"
#include "SubMesh.h"

class MeshClass
{
public:
	MeshClass();
    
	void SetMeshData(const SkinnedMeshData& skinnedMeshData);

	bool Initalize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext, unsigned int subMeshIndex);
	void Shutdown();
    
	bool IsInitalized() const { return mIsInitialize; }
	bool IsSkinning() const { return mIsSkinning; }

	unsigned int GetSubMeshCount() const { return mSubMeshs.size(); }
	string GetSubMeshName(int index) const { return mSubMeshs[index].GetName(); }
	unsigned int GetSubMeshIndexCount(int index) const { return mSubMeshs[index].GetIndexCount(); }
		 
	vector<Bone>& GetBones() { return bones; }
	Bone& GetBone(int index) { return bones[index]; }
private:
	bool mIsInitialize = false;
	bool mIsSkinning = false;

	std::vector<SubMesh> mSubMeshs;
	vector<Bone> bones;
};

