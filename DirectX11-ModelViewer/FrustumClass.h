#pragma once
#include"Bounds.h"
#include<DirectXMath.h>
using namespace DirectX;

class FrustumClass
{
public:
	FrustumClass() = default;
	~FrustumClass() = default;

	void ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);

	/// �������� �ȿ� �ٿ�� �ڽ� ������ True ��ȯ
	bool CheckBoundBox(const Bounds& bounds, const XMMATRIX& worldTransform);
private:
	XMVECTOR mPlanes[6];
};

