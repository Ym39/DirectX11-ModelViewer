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
    transform*= XMMatrixScaling(mScale.x,mScale.y,mScale.z) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x,mPosition.y,mPosition.z);
    return transform;
}

void Transform::Rotate(float delta, Axis axis = Axis::Yaxis)
{
    switch (axis)
    {
    case Axis::Xaxis:
        mRotation.x += delta;
        if (mRotation.x > 360.0f)
        {
            mRotation.x -= 360.0f;
        }
        else if (mRotation.x < 0.0f)
        {
            mRotation.x += 360.0f;
        }
        break;
    case Axis::Yaxis:
        mRotation.y += delta;
        if (mRotation.y > 360.0f)
        {
            mRotation.y -= 360.0f;
        }
        else if (mRotation.y < 0.0f)
        {
            mRotation.y += 360.0f;
        }
        break;
    case Axis::Zaxis:
        mRotation.z += delta;
        if (mRotation.z > 360.0f)
        {
            mRotation.z -= 360.0f;
        }
        else if (mRotation.z < 0.0f)
        {
            mRotation.z += 360.0f;
        }
        break;
    }
}

void Transform::Translate(XMFLOAT3 delta)
{
    mPosition = XMFLOAT3(mPosition.x + delta.x, mPosition.y + delta.y, mPosition.z + delta.z);
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

XMFLOAT3 Transform::Forward()
{
    XMFLOAT4X4 transform;
    XMStoreFloat4x4(&transform,XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
    XMVECTOR forward =XMVector3Normalize(XMVectorSet(transform._31, transform._32, transform._33, 0.0f));
    XMFLOAT3 forwardFinal;
    XMStoreFloat3(&forwardFinal, forward);
    return forwardFinal;
}

XMFLOAT3 Transform::Right()
{
    XMFLOAT4X4 transform;
    XMStoreFloat4x4(&transform, XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
    XMVECTOR right = XMVector3Normalize(XMVectorSet(transform._11, transform._12, transform._13, 0.0f));
    XMFLOAT3 rightFinal;
    XMStoreFloat3(&rightFinal, right);
    return rightFinal;
}

XMFLOAT3 Transform::Up()
{
    XMFLOAT4X4 transform;
    XMStoreFloat4x4(&transform, XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
    XMVECTOR up = XMVector3Normalize(XMVectorSet(transform._21, transform._22, transform._23, 0.0f));
    XMFLOAT3 upFinal;
    XMStoreFloat3(&upFinal, up);
    return upFinal;
}
