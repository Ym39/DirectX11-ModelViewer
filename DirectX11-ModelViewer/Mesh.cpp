#include "Mesh.h"

Mesh::Mesh():
mVertexBuffer(nullptr),
mIndexBuffer(nullptr),
mIsSkinnedMesh(false)
{

}

Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D11Device* device)
{
    bool result;

	if (vertices.empty() == true)
	{
	    return false;
	}

    result = InitializeBuffer(device);
    if (result == false)
    {
        return false;
    }

	return true;
}

void Mesh::Shutdown()
{
    ShutdownBuffers();
}

void Mesh::Render(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
	unsigned int offset;

	stride = sizeof(InputVertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Mesh::InitializeBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(InputVertex) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &(vertices.front());
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &(indices.front());
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Mesh::ShutdownBuffers()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	return;
}

void Mesh::SetMeshData(const vector<VertexType>& vertex, const vector<unsigned int>& index)
{
    vertices.resize(vertex.size());
    for (int i = 0; i < vertices.size(); i++)
    {
        for (int j = 0; j < vertex[i].blendingInfo.size(); ++j)
        {
            vertices[i].boneIndices[j] = vertex[i].blendingInfo[j].blendingIndex;
            switch (j)
            {
             case 0:
                 vertices[i].weight.x = vertex[i].blendingInfo[j].blendingIndex;
                 break;
			 case 1:
				 vertices[i].weight.y = vertex[i].blendingInfo[j].blendingIndex;
				 break;
			 case 2:
				 vertices[i].weight.z = vertex[i].blendingInfo[j].blendingIndex;
				 break;
            }
        }
    }

	indices.resize(index.size());
	copy(index.begin(),index.end(),indices.begin());
}

void Mesh::SetSkeleton(Skeleton* skeleton)
{
    mSkeleton = skeleton;
	mIsSkinnedMesh=true;
}
