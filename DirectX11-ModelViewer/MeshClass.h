#pragma once
#include <d3d11.h>
#include "Utility.h"
#include "Bounds.h"
#include "SubMeshGroup.h"

class MeshClass
{
public:
	MeshClass();
    
	void SetMeshData(const SkinnedMeshData& skinnedMeshData);

	bool Initalize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext, unsigned int submeshGroupIndex, unsigned int submeshIndex);
	void Shutdown();
    
	bool IsInitalized() const { return mIsInitialize; }
	bool IsSkinning() const { return mIsSkinning; }

	unsigned int GetSubMeshGroupCount() const { return mSubmeshGroups.size(); }
	string GetSubMeshGroupName(int index) const { return mSubmeshGroups[index].GetName(); }

	unsigned int GetSubMeshCount(int groupIndex) const { return mSubmeshGroups[groupIndex].GetSubmeshCount(); }
	unsigned int GetSubMeshIndexCount(int groupIndex, int submeshIndex) const { return mSubmeshGroups[groupIndex].GetSubmeshIndexCount(submeshIndex); }
		 
	vector<Bone>& GetBones() { return bones; }
	Bone& GetBone(int index) { return bones[index]; }
	const Bounds& GetBounds() const { return mBounds; }
private:
	void ComputeBounds();

private:
	bool mIsInitialize = false;
	bool mIsSkinning = false;

	std::vector<SubMeshGroup> mSubmeshGroups;
	vector<Bone> bones;
	Bounds mBounds;
};

