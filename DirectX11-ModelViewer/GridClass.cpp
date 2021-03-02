#include "GridClass.h"

GridClass::GridClass():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mVertexCount(0),
	mIndexCount(0)
{
}

GridClass::~GridClass()
{
}

bool GridClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
	if (result == false)
	{
		return false;
	}

	return true;
}

void GridClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

bool GridClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return true;
}

bool GridClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int gridWidth, gridHeight, index;
	XMFLOAT4 color;
	float positionX, positionZ;

	gridHeight = 256;
	gridWidth = 256;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	mVertexCount = (gridWidth - 1) * (gridHeight - 1) * 8;
	mIndexCount = mVertexCount;

	vertices = new VertexType[mVertexCount];

	indices = new unsigned long[mIndexCount];

	index = 0;

	for (int j = 0; j < (gridHeight - 1); j++)
	{
		for (int i = 0; i < (gridWidth - 1); i++)
		{
			//Line1-Upper left
			positionX = static_cast<float>(i);
			positionZ = static_cast<float>(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line1-Upper right
			positionX = static_cast<float>(i + 1);
			positionZ = static_cast<float>(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line2-Upper right
			positionX = static_cast<float>(i + 1);
			positionZ = static_cast<float>(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line2-Bottom right
			positionX = static_cast<float>(i + 1);
			positionZ = static_cast<float>(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line3-Bottom right
			positionX = static_cast<float>(i + 1);
			positionZ = static_cast<float>(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line3-Bottom Left
			positionX = static_cast<float>(i);
			positionZ = static_cast<float>(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line4-Bottom left
			positionX = static_cast<float>(i);
			positionZ = static_cast<float>(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			//Line4-Bottom right
			positionX = static_cast<float>(i);
			positionZ = static_cast<float>(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc,&vertexData,&mVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void GridClass::ShutdownBuffers()
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

void GridClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
