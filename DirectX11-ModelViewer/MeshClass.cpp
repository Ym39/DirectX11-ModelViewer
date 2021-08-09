#include "MeshClass.h"

MeshClass::MeshClass():
	mIsInitialize(false)
{
}

void MeshClass::SetMeshData(const SkinnedMeshData& skinnedMeshData)
{
	for (const auto& submeshGroups : skinnedMeshData.meshs)
	{
		SubMeshGroup current(submeshGroups.first, submeshGroups.second);
		mSubmeshGroups.push_back(current);
	}

	if (skinnedMeshData.bones.empty())
		return;

	mIsSkinning = true;
	bones.resize(skinnedMeshData.bones.size());
	for (int i = 0; i < bones.size(); i++)
	{
		bones[i] = skinnedMeshData.bones[i];
	}
}


bool MeshClass::Initalize(ID3D11Device* device)
{
	bool result;
	for (auto& submeshGroup : mSubmeshGroups)
	{
		result = submeshGroup.Initalize(device);
		if (result == false)
			return false;
	}
	mIsInitialize = true;
	return true;
}


void MeshClass::Render(ID3D11DeviceContext* deviceContext, unsigned int submeshGroupIndex, unsigned int submeshIndex)
{
	if (submeshGroupIndex >= mSubmeshGroups.size() || submeshIndex >= mSubmeshGroups[submeshGroupIndex].GetSubmeshIndexCount(submeshIndex))
		return;

	mSubmeshGroups[submeshGroupIndex].Render(deviceContext, submeshIndex);
}


void MeshClass::Shutdown()
{
	for (auto& submesh : mSubmeshGroups)
	{
		submesh.Shutdown();
	}
	return;
}
