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


