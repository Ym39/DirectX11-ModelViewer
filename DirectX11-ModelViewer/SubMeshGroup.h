#pragma once
#include <d3d11.h>
#include "Utility.h"
#include "SubMesh.h"

class SubMeshGroup
{
public:
	SubMeshGroup(string name, SaveMesh savemesh);

	bool Initalize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext, int index);
	void Shutdown();

	unsigned int GetSubmeshCount() const { return mSubmeshs.size(); }
	unsigned int GetSubmeshIndexCount(int index) const { return mSubmeshs[index].GetIndexCount(); }
	string GetName() const { return mName; }
private:
	vector<SubMesh> mSubmeshs;
	string mName = "";
};

