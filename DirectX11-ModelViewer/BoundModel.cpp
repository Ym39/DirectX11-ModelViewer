#include "BoundModel.h"

BoundModel::BoundModel(ID3D11Device* device,const Bounds& bounds):
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mVertexCount(0),
	mIndexCount(0)
{
	VertexType* vertices = new VertexType[8];

	XMFLOAT3 center = bounds.Center();
	XMFLOAT3 exents = bounds.Exents();

	//Up_LeftTop
	vertices[0].position = XMFLOAT3(center.x - exents.x,center.y + exents.y, center.z + exents.z);

	//Up_LeftBottom
	vertices[1].position = XMFLOAT3(center.x - exents.x, center.y + exents.y, center.z - exents.z);

	//Up_RightTop
	vertices[2].position = XMFLOAT3(center.x + exents.x, center.y + exents.y, center.z + exents.z);

	//Up_RightBottom
	vertices[3].position = XMFLOAT3(center.x + exents.x, center.y + exents.y, center.z - exents.z);

	//Down_LeftTop
	vertices[4].position = XMFLOAT3(center.x - exents.x, center.y - exents.y, center.z + exents.z);

	//Down_LeftBottom
	vertices[5].position = XMFLOAT3(center.x - exents.x, center.y - exents.y, center.z - exents.z);

	//Down_RightTop
	vertices[6].position = XMFLOAT3(center.x + exents.x, center.y - exents.y, center.z + exents.z);

	//Down_RightBottom
	vertices[7].position = XMFLOAT3(center.x + exents.x, center.y - exents.y, center.z - exents.z);

	unsigned long indices[24]=
	{
		0,1,
		0,2,
		1,3,
		2,3,
		0,4,
		1,5,
		2,6,
		3,7,
		4,5,
		4,6,
		6,7,
		5,7
	};

	mVertexCount = 8;
	mIndexCount = 24;

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		mVertexBuffer = nullptr;
		return;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		mIndexBuffer = nullptr;
		return;
	}

	delete[] vertices;
	vertices = nullptr;
}

BoundModel::~BoundModel()
{
	ShutdownBuffers();
}

bool BoundModel::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return true;
}

void BoundModel::ShutdownBuffers()
{
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

void BoundModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}
