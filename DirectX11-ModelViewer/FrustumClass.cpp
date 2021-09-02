#include "FrustumClass.h"

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	XMFLOAT4X4 projection;
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&projection, projectionMatrix);
	XMStoreFloat4x4(&view, viewMatrix);

	zMinimum = -projection._43 / projection._33;
	r = screenDepth / (screenDepth - zMinimum);
	projection._33 = r;
	projection._43 = -r * zMinimum;
	projectionMatrix = XMLoadFloat4x4(&projection);

	XMMATRIX finalMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, finalMatrix);

	//near plane
	float x = (float)(matrix._14 + matrix._13);
	float y = (float)(matrix._24 + matrix._23);
	float z = (float)(matrix._34 + matrix._33);
	float w = (float)(matrix._44 + matrix._43);
	mPlanes[0] = XMVectorSet(x, y, z, w);
	mPlanes[0] = XMPlaneNormalize(mPlanes[0]);

	//far plane
	x = (float)(matrix._14 - matrix._13);
	y = (float)(matrix._24 - matrix._23);
	z = (float)(matrix._34 - matrix._33);
	w = (float)(matrix._44 - matrix._43);
	mPlanes[1] = XMVectorSet(x, y, z, w);
	mPlanes[1] = XMPlaneNormalize(mPlanes[1]);

	//left plane
	x = (float)(matrix._14 + matrix._11);
	y = (float)(matrix._24 + matrix._21);
	z = (float)(matrix._34 + matrix._31);
	w = (float)(matrix._44 + matrix._41);
	mPlanes[2] = XMVectorSet(x, y, z, w);
	mPlanes[2] = XMPlaneNormalize(mPlanes[2]);

	//right plane
	x = (float)(matrix._14 - matrix._11);
	y = (float)(matrix._24 - matrix._21);
	z = (float)(matrix._34 - matrix._31);
	w = (float)(matrix._44 - matrix._41);
	mPlanes[3] = XMVectorSet(x, y, z, w);
	mPlanes[3] = XMPlaneNormalize(mPlanes[3]);

	//bottom plane
	x = (float)(matrix._14 + matrix._12);
	y = (float)(matrix._24 + matrix._22);
	z = (float)(matrix._34 + matrix._32);
	w = (float)(matrix._44 + matrix._42);
	mPlanes[4] = XMVectorSet(x, y, z, w);
	mPlanes[4] = XMPlaneNormalize(mPlanes[4]);

	//top plane
	x = (float)(matrix._14 - matrix._12);
	y = (float)(matrix._24 - matrix._22);
	z = (float)(matrix._34 - matrix._32);
	w = (float)(matrix._44 - matrix._42);
	mPlanes[5] = XMVectorSet(x, y, z, w);
	mPlanes[5] = XMPlaneNormalize(mPlanes[5]);
}

bool FrustumClass::CheckBoundBox(const Bounds& bounds,const XMMATRIX& worldTransform)
{
	XMFLOAT3 center = bounds.Center();
	XMFLOAT3 extens = bounds.Exents();
	XMFLOAT3 min(center.x - extens.x, center.y - extens.y, center.z - extens.z);
	XMFLOAT3 max(center.x + extens.x, center.y + extens.y, center.z + extens.z);

	XMStoreFloat3(&min, XMVector3TransformCoord(XMVectorSet(min.x, min.y, min.z, 0.0f), worldTransform));
	XMStoreFloat3(&max, XMVector3TransformCoord(XMVectorSet(max.x, max.y, max.z, 0.0f), worldTransform));

	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(mPlanes[i], XMVectorSet(min.x, min.y, min.z, 1.0f))) >= 0.0f)
			continue;
		if (XMVectorGetX(XMPlaneDotCoord(mPlanes[i], XMVectorSet(max.x, max.y, max.z, 1.0f))) >= 0.0f)
			continue;

		return false;
	}
    
	return true;
}
