#include "ArrowModel.h"

ArrowModel::ArrowModel():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr)
{
}

ArrowModel::~ArrowModel()
{
}

bool ArrowModel::Initialize(ID3D11Device* device, ArrowDirection direction,XMFLOAT3 scale)
{
	mArrowDirection = direction;

	if (InitializeBuffers(device,scale) == false)
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
	/*center.x += position.x;
	center.y += position.y;
	center.z += position.z;*/

	XMFLOAT3 extens = mBounds.Exents();
	XMFLOAT3 min;
	min.x = center.x - extens.x * arrowScale.x;
	min.y = center.y - extens.y * arrowScale.y;
	min.z = center.z - extens.z * arrowScale.z;

	XMFLOAT3 max;
	max.x = center.x + extens.x * arrowScale.x;
	max.y = center.y + extens.y * arrowScale.y;
	max.z = center.z + extens.z * arrowScale.z;

	/*float tmin = (min.x - rayOrigin.x) / rayDirection.x;
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
	
	if ((tmin < tzmax) || (tzmin < tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;
 
	return true;*/

	XMFLOAT3 dirfrac(1.f / rayDirection.x, 1.f / rayDirection.y, 1.f / rayDirection.z);

	float t1 = (min.x - rayOrigin.x) * dirfrac.x;
	float t2 = (max.x - rayOrigin.x) * dirfrac.x;
	float t3 = (min.y - rayOrigin.y) * dirfrac.y;
	float t4 = (max.y - rayOrigin.y) * dirfrac.y;
	float t5 = (min.z - rayOrigin.z) * dirfrac.z;
	float t6 = (max.z - rayOrigin.z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	float t;
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	return true;

	/*float a = (rayDirection.x * rayDirection.x) + (rayDirection.y + rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - ((extens.z * arrowScale.z) * (extens.z * arrowScale.z));

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;*/
}

bool ArrowModel::InitializeBuffers(ID3D11Device* device, XMFLOAT3 scale)
{
	unsigned long* indices = new unsigned long[arrowIndice.size()];
	for (int i = 0; i < arrowIndice.size(); i++)
	{
		indices[i] = arrowIndice[i];
	}

	std::array<XMFLOAT3, 29> position;
	std::copy(arrowPositions.begin(), arrowPositions.end(), position.begin());

	switch (mArrowDirection)
	{
	case ArrowDirection::Forward:
		for (auto& curPos : position)
			XMStoreFloat3(&curPos, (XMVector3Transform(XMLoadFloat3(&curPos),XMMatrixScaling(scale.x, scale.y, scale.z))));
		break;
	case ArrowDirection::Right:
		for(auto& curPos : position)
		   XMStoreFloat3(&curPos,(XMVector3Transform(XMLoadFloat3(&curPos), XMMatrixRotationY(XMConvertToRadians(90.f))*XMMatrixScaling(scale.x, scale.y, scale.z))));
		break;
	case ArrowDirection::Up:
		for (auto& curPos : position)
			XMStoreFloat3(&curPos, (XMVector3Transform(XMLoadFloat3(&curPos), XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixScaling(scale.x, scale.y, scale.z))));
		break;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * arrowPositions.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(position.front());
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
