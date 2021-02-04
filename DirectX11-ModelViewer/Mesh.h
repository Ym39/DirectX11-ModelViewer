#pragma once
#include <d3d11.h>
#include "Utility.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool Initialize(ID3D11Device* device);
    void Shutdown();
    void Render(ID3D11DeviceContext* deviceContext);

	void SetMeshData(const vector<VertexType>& vertex, const vector<unsigned int>& index);
	void SetSkeleton(Skeleton* skeleton);
private:
    bool InitializeBuffer(ID3D11Device* device);
    void ShutdownBuffers();
   

    vector<InputVertex>& GetVertices() { return vertices;}
    vector<unsigned int>& GetIndices() {return indices;}
private:
    ID3D11Buffer* mVertexBuffer,*mIndexBuffer;
    int mVertexCount, mIndexCount;

    vector<InputVertex> vertices;
    vector<unsigned int> indices;

    Skeleton* mSkeleton;

    bool mIsSkinnedMesh;
};

