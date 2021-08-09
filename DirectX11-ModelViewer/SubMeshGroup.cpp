#include "SubMeshGroup.h"

SubMeshGroup::SubMeshGroup(string name, SaveMesh savemesh)
	:mName(name)
{
	mSubmeshs.reserve(savemesh.submeshs.size());

	for (int i = 0; i < savemesh.submeshs.size(); i++)
	{
		SubMesh submesh(savemesh.submeshs[i]);
		mSubmeshs.push_back(submesh);
	}
}

bool SubMeshGroup::Initalize(ID3D11Device* device)
{
	for (auto& submesh : mSubmeshs)
	{
		if (submesh.Initalize(device) == false)
			return false;
	}

	return true;
}

void SubMeshGroup::Render(ID3D11DeviceContext* deviceContext, int index)
{
	if (index >= mSubmeshs.size())
		return;

	mSubmeshs[index].Render(deviceContext);
}

void SubMeshGroup::Shutdown()
{
	for (auto& submesh : mSubmeshs)
	{
		submesh.Shutdown();
	}
}
