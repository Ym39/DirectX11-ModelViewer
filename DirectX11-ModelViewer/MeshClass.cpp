#include "MeshClass.h"

MeshClass::MeshClass():
	mIsInitialize(false)
{
}

void MeshClass::SetMeshData(const SkinnedMeshData& skinnedMeshData)
{
	for (const auto& submesh : skinnedMeshData.meshs)
	{
		SubMesh current(submesh.first, submesh.second);
		mSubMeshs.push_back(current);
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
	for (auto& submesh : mSubMeshs)
	{
		result = submesh.Initalize(device);
		if (result == false)
			return false;
	}
	mIsInitialize = true;
	return true;
}


void MeshClass::Render(ID3D11DeviceContext* deviceContext, unsigned int subMeshIndex)
{
	if (mSubMeshs.size() <= subMeshIndex)
		return;

	mSubMeshs[subMeshIndex].Render(deviceContext);
}


void MeshClass::Shutdown()
{
	for (auto& submesh : mSubMeshs)
	{
		submesh.Shutdown();
	}
	return;
}
