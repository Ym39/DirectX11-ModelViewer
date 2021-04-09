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

	ComputeBounds();

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

bool ArrowModel::RayIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMFLOAT3 position, XMFLOAT3 arrowScale)
{
	XMFLOAT3 center = mBounds.Center();
	center.x += position.x;
	center.y += position.y;
	center.z += position.z;

	XMFLOAT3 extens = mBounds.Exents();
	XMFLOAT3 min;
	min.x = center.x - extens.x * arrowScale.x;
	min.y = center.y - extens.y * arrowScale.y;
	min.z = center.z - extens.z * arrowScale.z;

	XMFLOAT3 max;
	max.x = center.x + extens.x * arrowScale.x;
	max.y = center.y + extens.y * arrowScale.y;
	max.z = center.z + extens.z * arrowScale.z;

	float tmin = (min.x - rayOrigin.x) / rayDirection.x;
	float tmax = (max.x - rayOrigin.x) / rayDirection.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - rayOrigin.y) / rayDirection.y;
	float tymax = (max.y - rayOrigin.y) / rayDirection.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - rayOrigin.z) / rayDirection.z;
	float tzmax = (max.z - rayOrigin.z) / rayDirection.z;

	if (tzmin > tzmax)swap(tzmin, tzmax);
	
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;
 
	return true;
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

void ArrowModel::ComputeBounds()
{
	float maxX = 0.f, maxY = 0.f, maxZ = 0.f, minX = 0.f, minY = 0.f, minZ = 0.f;

	for (const auto& vertex : arrowPositions)
	{
		if (maxX < vertex.x) maxX = vertex.x;
		if (maxY < vertex.y) maxY = vertex.y;
		if (maxZ < vertex.z) maxZ = vertex.z;
		if (minX > vertex.x) minX = vertex.x;
		if (minY > vertex.y) minY = vertex.y;
		if (minZ > vertex.z) maxZ = vertex.z;
	}

	XMFLOAT3 center((maxX + minX) * 0.5f, (maxY + minY) * 0.5f, (maxZ + minZ) * 0.5f);
	mBounds.SetCenter(center);
	XMFLOAT3 exents(maxX - center.x,maxY-center.y,maxZ-center.z);
	mBounds.SetExents(exents);
}
