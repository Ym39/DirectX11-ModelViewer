#pragma once
#include <directxmath.h>
#include"Bounds.h"
#include <minwindef.h>
using namespace DirectX;
namespace Physics
{
	bool RayIntersectWithBoundsBox(XMFLOAT3 start, XMFLOAT3 direction, const XMMATRIX& transformMatrix , const Bounds& bounds)
	{
		XMFLOAT3 center = bounds.Center();
		XMFLOAT3 extens = bounds.Exents();
		XMFLOAT3 min;
		min.x = center.x - extens.x;
		min.y = center.y - extens.y;
		min.z = center.z - extens.z;

		XMFLOAT3 max;
		max.x = center.x + extens.x;
		max.y = center.y + extens.y;
		max.z = center.z + extens.z;

		XMStoreFloat3(&min, XMVector3TransformCoord(XMVectorSet(min.x, min.y, min.z, 0.0f), transformMatrix));
		XMStoreFloat3(&max, XMVector3TransformCoord(XMVectorSet(max.x, max.y, max.z, 0.0f), transformMatrix));

		XMFLOAT3 dirfrac(1.f / direction.x, 1.f / direction.y, 1.f / direction.z);

		float t1 = (min.x - start.x) * dirfrac.x;
		float t2 = (max.x - start.x) * dirfrac.x;
		float t3 = (min.y - start.y) * dirfrac.y;
		float t4 = (max.y - start.y) * dirfrac.y;
		float t5 = (min.z - start.z) * dirfrac.z;
		float t6 = (max.z - start.z) * dirfrac.z;

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
	}

	bool TestIntersectionBoundBox(int mouseX, int mouseY, int screenWidth, int screenHeight, XMFLOAT3 cameraPosition, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const XMMATRIX& transformMatrix, const Bounds& bounds)
	{
		XMMATRIX inverseViewMatrix, inverseWorldMatrix;
		XMFLOAT3 direction, rayOrigin, rayDirection;

		float pointX = ((2.0f * (float)mouseX) / (float)screenWidth) - 1.0f;
		float pointY = (((2.0f * (float)mouseY) / (float)screenHeight) - 1.0f) * -1.0f;

		XMFLOAT3X3 projectionMatrix4;
		XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

		pointX = pointX / projectionMatrix4._11;
		pointY = pointY / projectionMatrix4._22;

		inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

		XMFLOAT3X3 inverseViewMatrix4;
		XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

		direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
		direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
		direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

		inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

		XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.f), inverseWorldMatrix));
		XMStoreFloat3(&rayDirection, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.f), inverseWorldMatrix));

		XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.f)));

		return RayIntersectWithBoundsBox(rayOrigin, rayDirection, transformMatrix, bounds);
	}
}