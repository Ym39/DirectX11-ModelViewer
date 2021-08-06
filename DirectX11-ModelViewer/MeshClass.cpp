#include "MeshClass.h"

MeshClass::MeshClass():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mIsInitialize(false)
{
}

MeshClass& MeshClass::operator=(const SkinnedMeshData& meshData)
{
	if (meshData.vertices.empty() || meshData.indices.empty())
		return *this;

	vertices.resize(meshData.vertices.size());
	//copy(meshData.vertices.begin(), meshData.vertices.end(), vertices.begin());
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = meshData.vertices[i];
	}

	indices.resize(meshData.indices.size());
	copy(meshData.indices.begin(), meshData.indices.end(), indices.begin());

	if (meshData.bones.empty())
	{
		return *this;
	}

	mIsSkinning = true;
	bones.resize(meshData.bones.size());
	//copy(meshData.bones.begin(), meshData.bones.end(), bones.begin());
	for (int i = 0; i < bones.size(); i++)
	{
		bones[i] = meshData.bones[i];
	}

	return *this;
}

bool MeshClass::Initalize(ID3D11Device* device)
{
	if (vertices.empty() == true || indices.empty() == true)
		return false;

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

	mIsInitialize = true;
	return true;
}

void MeshClass::Render(ID3D11DeviceContext* deviceContext)
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

void MeshClass::Render(ID3D11DeviceContext* deviceContext, UINT bufferNumber)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(InputVertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(bufferNumber, 1, &mVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void MeshClass::Shutdown()
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
}
