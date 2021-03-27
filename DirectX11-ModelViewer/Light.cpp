#include "Light.h"

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	mAmbientColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	mDiffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	mSpecularColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetSpecularPower(float power)
{
	mSpecularPower = power;
}

void Light::SetDirection(float x, float y, float z)
{
	mDirection = XMFLOAT3(x, y, z);
}

void Light::GenerateViewMatrix()
{
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.f);

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR position = XMLoadFloat3(&mPosition);
	XMVECTOR lookAt = XMLoadFloat3(new XMFLOAT3(0.0f, 0.0f, 1.0f));

	mViewMatrix = XMMatrixLookAtLH(position, lookAt, upVector);
}

void Light::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView = (float)XM_PI / 2.0f;
	float screenAspect = 1.0f;

	mProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
}

void Light::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = mViewMatrix;
}

void Light::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = mProjectionMatrix;
}


