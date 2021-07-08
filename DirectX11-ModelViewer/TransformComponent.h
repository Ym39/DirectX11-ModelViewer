#pragma once
#include "Component.h"
#include <directxmath.h>
using namespace DirectX;

class TransformComponent : public BaseComponent
{
public:
	DEFINE_COMPONENT(TransformComponent, BaseComponent, true);

public:
	TransformComponent() :
		mPosition(0.f, 0.f, 0.f),
		mRotation(0.f, 0.f, 0.f),
		mScale(1.f, 1.f, 1.f)
	{
		UpdateTransform();
	}
	~TransformComponent() = delete;

public:
	virtual void Start() override {};
	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Destroy() override {};

	XMVECTOR GetPosition() const { return XMLoadFloat3(&mPosition); }
	XMFLOAT3 GetRotation() const { return mRotation; }
	XMFLOAT3 GetScale() const { return mScale; }
	XMMATRIX GetTransform() const { return mTransform; }

	void SetPosition(XMFLOAT3 position)
	{
		mPosition = position;
		UpdateTransform();
	}

	void SetRotation(XMFLOAT3 rotation)
	{
		mRotation = rotation;
		UpdateTransform();
	}

	void SetScale(XMFLOAT3 scale)
	{
		mScale = scale;
		UpdateTransform();
	}

	void UpdateTransform()
	{
		mTransform = XMMatrixScaling(mScale.x, mScale.y, mScale.z)
			* XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}


	XMVECTOR Forward() {return XMVector3Normalize(mTransform.r[3]);}
	XMVECTOR Right() {	return XMVector3Normalize(mTransform.r[1]);}
	XMVECTOR Up() {return XMVector3Normalize(mTransform.r[2]);}

private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
	XMMATRIX mTransform;
};

DECLARE_COMPONENT(TransformComponent);