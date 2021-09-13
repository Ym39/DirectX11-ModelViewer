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

	ComputeBounds();

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

void MeshClass::ComputeBounds()
{
	float minX = 0.0f, maxX = 0.0f;
	float minY = 0.0f, maxY = 0.0f;
	float minZ = 0.0f, maxZ = 0.0f;

	for (auto& submeshGroup : mSubmeshGroups)
	{
		int numOfsubmesh = submeshGroup.GetSubmeshCount();
		for (int submeshCount = 0; submeshCount < numOfsubmesh; submeshCount++)
		{
			SubMesh* submesh = nullptr;
			submeshGroup.GetSubMesh(submeshCount, &submesh);
			if (submesh != nullptr)
			{
				const auto& vertices = submesh->GetVertices();
				for (const auto& vertex : vertices)
				{
					minX = minX > vertex.position.x ? vertex.position.x : minX;
					maxX = maxX < vertex.position.x ? vertex.position.x : maxX;
					minY = minY > vertex.position.y ? vertex.position.y : minY;
					maxY = maxY < vertex.position.y ? vertex.position.y : maxY;
					minZ = minZ > vertex.position.z ? vertex.position.z : minZ;
					maxZ = maxZ < vertex.position.z ? vertex.position.z : maxZ;
				}
			}
		}
	}

	XMFLOAT3 center((maxX + minX) * 0.5f, (maxY + minY) * 0.5f, (maxZ + minZ) * 0.5f);
	mBounds.SetCenter(center);
	XMFLOAT3 exents(maxX - center.x, maxY - center.y, maxZ - center.z);
	mBounds.SetExents(exents);

}
