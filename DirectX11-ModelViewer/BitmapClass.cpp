#include "BitmapClass.h"

bool BitmapClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int bitmapWidth, int bitmapHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mBitmapHeight = bitmapHeight;
	mBitmapWidth = bitmapWidth;

	mPreviousPosX = -1;
	mPreviousPosY = -1;

	bool result = InitializeBuffers(device);
	if (!result)
		return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
		return false;

	return true;
}

void BitmapClass::Shutdown()
{
	ShutdownBuffers();

	ReleaseTexture();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
		return false;

	RenderBuffers(deviceContext);

	return true;
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	mVertexCount = 6;
	mIndexCount = 6;

	VertexType* vertices = new VertexType[mVertexCount];
	if (!vertices)
		return false;

	unsigned long* indices = new unsigned long[mIndexCount];
	if (!indices)
		return false;

	memset(vertices, 0, (sizeof(VertexType) * mVertexCount));

	for (int i = 0; i < mIndexCount; ++i)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	if ((positionX == mPreviousPosX) && (positionY == mPreviousPosY))
		return true;

	mPreviousPosX = positionX;
	mPreviousPosY = positionY;

	float left = (float)(mScreenWidth * -0.5) + positionX;
	float right = left + (float)mBitmapWidth;
	float top = (float)(mScreenHeight * 0.5) - positionY;
	float bottom = top - (float)mBitmapHeight;

	VertexType* vertices = new VertexType[mVertexCount];
	if (!vertices)
		return false;

	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	//vertices[0].position = PointToNdc(left, top);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	//vertices[1].position = PointToNdc(right, bottom);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	//vertices[2].position = PointToNdc(left, bottom);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	//vertices[3].position = PointToNdc(left, top);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	//vertices[4].position = PointToNdc(right, top);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	//vertices[5].position = PointToNdc(right, bottom);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	//float tx = 0.5f * (vertices[0].position.x + vertices[3].position.x);
	//float ty = 0.5f * (vertices[0].position.y + vertices[1].position.y);

	/*XMVECTOR scaling = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR origin = XMVectorSet(tx, ty, 0.0f, 0.f);
	XMVECTOR translation = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	XMMATRIX transform = XMMatrixAffineTransformation2D(scaling, origin, 0.0f, translation);

	for (int i = 0; i < 6; i++)
	{
		XMVECTOR position = XMLoadFloat3(&vertices[i].position);
		position = XMVector3TransformCoord(position, transform);
		XMStoreFloat3(&vertices[i].position, position);
	}*/

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	VertexType* verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * mVertexCount);
	deviceContext->Unmap(mVertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

XMFLOAT3 BitmapClass::PointToNdc(int x, int y)
{
	return XMFLOAT3(2.0f * (float)x / mScreenWidth - 1.0f,
		1.0f - 2.0f * (float)y/mScreenHeight,0.0f);
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	mTexture = new Texture;
	if (!mTexture)
		return false;

	bool result = mTexture->Initialize(device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

void BitmapClass::ReleaseTexture()
{
	if (mTexture)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = nullptr;
	}
}

BitmapClass::BitmapClass():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr)
{
}
