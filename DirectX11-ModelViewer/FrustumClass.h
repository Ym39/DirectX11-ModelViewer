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

	/// 프러스텀 안에 바운드 박스 있으면 True 반환
	bool CheckBoundBox(const Bounds& bounds, const XMMATRIX& worldTransform);
private:
	XMVECTOR mPlanes[6];
};

