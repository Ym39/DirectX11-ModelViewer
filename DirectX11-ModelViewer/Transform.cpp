#include "Transform.h"

Transform::Transform()
{
    mPosition.x = mPosition.y = mPosition.z =0.0f;
    mRotation.x = mRotation.y = mRotation.z = 0.0f;
    mScale.x = mScale.y = mScale.z = 1.0f;
}

DirectX::XMFLOAT3 Transform::GetPosition() const
{
    return mPosition;
}

XMFLOAT3 Transform::GetRotation() const
{
    return mRotation;
}

XMFLOAT3 Transform::GetScale() const
{
    return mScale;
}

DirectX::XMMATRIX Transform::GetTransform() const
{
    XMMATRIX transform;
    transform = XMMatrixIdentity();
    transform*= XMMatrixScaling(mScale.x,mScale.y,mScale.z) * XMMatrixRotationRollPitchYaw(mRotation.x,mRotation.y,mRotation.z) * XMMatrixTranslation(mPosition.x,mPosition.y,mPosition.z);
    return transform;
}

void Transform::SetPosition(XMFLOAT3 position)
{
    mPosition = position;
}

void Transform::SetRotation(XMFLOAT3 rotation)
{
    mRotation = rotation;
}

void Transform::SetScale(XMFLOAT3 scale)
{
    mScale = scale;
}
