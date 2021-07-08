#pragma once
#include<DirectXMath.h>
using namespace DirectX;

class Light
{
public:
	Light() = default;
	Light(const Light&) = default;
	~Light() = default;

public:
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetPosition(float x, float y, float z);
	void SetSpecularColor(float red, float green, float blue, float alpha);
	void SetSpecularPower(float power);
	void SetDirection(float x, float y , float z);

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float screenDepth, float screenNear);

	XMFLOAT4 GetAmbientColor() const { return mAmbientColor; }
	XMFLOAT4 GetDiffuseColor() const { return mDiffuseColor; }
	XMFLOAT3 GetPosition() const { return mPosition; }
	XMFLOAT4 GetSpecularColor() const { return mSpecularColor; }
	float GetSpecularPower() const { return mSpecularPower;}
	XMFLOAT3 GetDirection() const { return mDirection; }

	void GetViewMatrix(XMMATRIX& viewMatrix);
	void GetProjectionMatrix(XMMATRIX& projectionMatrix);

private:
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT3 mPosition;
	XMFLOAT4 mSpecularColor;
	XMFLOAT3 mDirection;
	float mSpecularPower;
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;
};
