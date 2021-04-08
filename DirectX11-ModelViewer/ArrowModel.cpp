#include "ArrowModel.h"

ArrowModel::ArrowModel():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr)
{
}

ArrowModel::~ArrowModel()
{
}

bool ArrowModel::Initialize(ID3D11Device* device)
{
	if (InitializeBuffers(device) == false)
		return false;

	mVertexCount = arrowPositions.size();
	mIndexCount = arrowIndice.size();

	return true;
}

void ArrowModel::Shutdown()
{
	ShutdownBuffers();
}

void ArrowModel::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

bool ArrowModel::InitializeBuffers(ID3D11Device* device)
{
	unsigned long* indices = new unsigned long[arrowIndice.size()];
	for (int i = 0; i < arrowIndice.size(); i++)
	{
		indices[i] = arrowIndice[i];
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * arrowPositions.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(arrowPositions.front());
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer)))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * arrowIndice.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer)))
	{
		return false;
	}

	delete[] indices;
	indices = 0;

	return true;
}

void ArrowModel::ShutdownBuffers()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}
}

void ArrowModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
