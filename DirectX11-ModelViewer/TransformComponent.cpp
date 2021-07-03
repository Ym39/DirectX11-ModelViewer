#include "TransformComponent.h"

TransformComponent::TransformComponent():
	mPosition(0.f,0.f,0.f),
	mRotation(0.f,0.f,0.f),
	mScale(1.f,1.f,1.f)
{
	UpdateTransform();
}

void TransformComponent::Start()
{
}

void TransformComponent::Update()
{
}

void TransformComponent::LateUpdate()
{
}

void TransformComponent::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
	UpdateTransform();
}

void TransformComponent::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
	UpdateTransform();
}

void TransformComponent::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
	UpdateTransform();
}

void TransformComponent::UpdateTransform()
{
	mTransform = XMMatrixScaling(mScale.x, mScale.y, mScale.z)
		* XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}

XMVECTOR TransformComponent::Forward()
{
	return XMVector3Normalize(mTransform.r[3]);
}

XMVECTOR TransformComponent::Right()
{
	return XMVector3Normalize(mTransform.r[1]);
}

XMVECTOR TransformComponent::Up()
{
	return XMVector3Normalize(mTransform.r[2]);
}
