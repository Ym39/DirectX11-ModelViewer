#pragma once
#include <directxmath.h>
using namespace DirectX;
using namespace std;

class Bounds
{
public:
	Bounds();
	~Bounds() = default;
    
	void SetCenter(XMFLOAT3 center) { mCenter = center; }
	void SetExents(XMFLOAT3 exents) 
	{
		mExents = exents; 
		mMin = XMFLOAT3(mCenter.x-mExents.x, mCenter.y - mExents.y, mCenter.z - mExents.z);
		mMax = XMFLOAT3(mCenter.x + mExents.x, mCenter.y + mExents.y, mCenter.z + mExents.z);
	}

	XMFLOAT3 Center() const { return mCenter; }
	XMFLOAT3 Exents() const { return mExents; }
	XMFLOAT3 Min() const { return mMin; }
	XMFLOAT3 Max() const { return mMax; }
private:
	XMFLOAT3 mCenter;
	XMFLOAT3 mExents;
	XMFLOAT3 mMin;
	XMFLOAT3 mMax;
};

