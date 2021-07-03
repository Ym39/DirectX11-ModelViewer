#pragma once
#include <d3d11.h>
#include "Utility.h"

class MeshClass
{
public:
	MeshClass();
    
private:
	ID3D11Buffer* mVertexBuffer, * mIndexBuffer;
	vector<InputVertex> vertices;
	vector<unsigned int> indices;
	vector<Bone> bones;
};

