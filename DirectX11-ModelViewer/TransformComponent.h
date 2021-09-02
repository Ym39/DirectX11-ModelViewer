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
		mScale(1.f, 1.f, 1.f),
		mLocalPosition(0.0f, 0.0f, 0.0f),
		mLocalRotation(0.0f, 0.0f, 0.0f),
		mLocalScale(1.f,1.f,1.f),
		mParent(nullptr),
		mChild(nullptr)
	{
		//UpdateTransform();
	}
	~TransformComponent() = delete;

public:
	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override {};
	virtual void Destroy() override {};

	XMVECTOR GetPosition() const { return XMLoadFloat3(&mPosition); }
	XMFLOAT3 GetRotation() const { return mRotation; }
	XMFLOAT3 GetScale() const { return mScale; }
	XMFLOAT3 GetLocalPosition() const { return mLocalPosition; }
	XMFLOAT3 GetLocalRotation() const { return mLocalRotation; }
	XMFLOAT3 GetLocalScale() const { return mLocalScale; }
	XMMATRIX GetTransform() const
	{ 
	    return XMMatrixScaling(mScale.x, mScale.y, mScale.z) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}
	void Translate(XMFLOAT3 delta) 
	{ 
		mPosition.x += delta.x; mPosition.y += delta.y; mPosition.z += delta.z; 
		if (mParent != nullptr)
			WorldToLocal(mParent);
		else
			mLocalPosition = mPosition;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetPosition(XMFLOAT3 position)
	{
		mPosition = position;
		if (mParent != nullptr)
			WorldToLocal(mParent);
		else
			mLocalPosition = mPosition;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetLocalPosition(XMFLOAT3 position)
	{
		mLocalPosition = position;
		if (mParent != nullptr)
			LocalToWorld(mParent);
		else
			mPosition = mLocalPosition;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetRotation(XMFLOAT3 rotation)
	{
		mRotation = rotation;
		if (mParent != nullptr)
			WorldToLocal(mParent);
		else
			mLocalRotation = mRotation;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetLocalRotation(XMFLOAT3 rotation)
	{
		mLocalRotation = rotation;
		if (mParent != nullptr)
			LocalToWorld(mParent);
		else
			mRotation = mLocalRotation;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetScale(XMFLOAT3 scale)
	{
		mScale = scale;
		if (mParent != nullptr)
			WorldToLocal(mParent);
		else
			mLocalScale = mScale;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void SetLocalScale(XMFLOAT3 scale)
	{
		mLocalScale = scale;
		if (mParent != nullptr)
			LocalToWorld(mParent);
		else
			mScale = mLocalScale;

		if (mChild != nullptr)
			mChild->LocalToWorld(this);
	}

	void UpdateTransform()
	{
		mTransform = XMMatrixScaling(mScale.x, mScale.y, mScale.z)
			* XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}

	void SetParent(TransformComponent* parent)
	{
		if (parent == nullptr)
		{
			mParent->SetChild(nullptr);
			mParent = nullptr;
			mLocalPosition = mPosition;
			mLocalRotation = mRotation;
			mLocalScale = mScale;
		}
		else
		{
			mParent = parent;
			mParent->SetChild(this);
			WorldToLocal(mParent);
		}
	}

	void SetChild(TransformComponent* parent)
	{
		mChild = parent;
	}

	TransformComponent* GetParent() const { return mParent; }

	XMVECTOR Forward() {return XMVector3Normalize(mTransform.r[3]);}
	XMVECTOR Right() {	return XMVector3Normalize(mTransform.r[1]);}
	XMVECTOR Up() {return XMVector3Normalize(mTransform.r[2]);}

private:

	XMMATRIX LocalToWorld(const TransformComponent* parent) 
	{
		XMFLOAT3 parentRotation =parent->GetRotation();
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(parentRotation.x, parentRotation.y, parentRotation.z);
		XMFLOAT3 parentScale = parent->GetScale();
		XMFLOAT3 parentPosition;
		XMStoreFloat3(&parentPosition, parent->GetPosition());

		XMFLOAT3 rotation(mLocalRotation.x + parentRotation.x, mLocalRotation.y + parentRotation.y, mLocalRotation.z + parentRotation.z);
		mRotation = rotation;
		//SetRotation(rotation);
		XMFLOAT3 scale(mLocalScale.x * parentScale.x, mLocalScale.y * parentScale.y, mLocalScale.z * parentScale.z);
		//SetScale(scale);
		mScale = scale;
		
		XMFLOAT3 position;
		position.x = mLocalPosition.x * parentScale.x;
		position.y = mLocalPosition.y * parentScale.y;
		position.z = mLocalPosition.z * parentScale.z;
		XMVECTOR posVector = XMLoadFloat3(&position);
		XMVector3Transform(posVector, rotMat);
		XMStoreFloat3(&position, posVector);
		position.x += parentPosition.x;
		position.y += parentPosition.y;
		position.z += parentPosition.z;

		mPosition = position;
		//SetPosition(position);

		return XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z)) * XMMatrixTranslation(position.x, position.y, position.z);
	}

	void WorldToLocal(TransformComponent* parent)
	{
		if (parent == nullptr)
		   return;

		XMFLOAT3 parentRotation = parent->GetRotation();
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(parentRotation.x, parentRotation.y, parentRotation.z);
		rotMat = XMMatrixInverse(nullptr, rotMat);
		XMFLOAT3 parentScale = parent->GetScale();
		XMFLOAT3 parentPosition;
		XMStoreFloat3(&parentPosition, parent->GetPosition());


		XMFLOAT3 scale;
		scale.x = 1 / parentScale.x * mScale.x;
		scale.y = 1 / parentScale.y * mScale.y;
		scale.z = 1 / parentScale.z * mScale.z;
		mLocalScale = scale;

		//SetScale(scale);
		XMFLOAT3 rotation;
		rotation.x = mRotation.x - parentRotation.x;
		rotation.y = mRotation.y - parentRotation.y;
		rotation.z = mRotation.z - parentRotation.z;
		mLocalRotation = rotation;
		//SetScale(rotation);

		XMFLOAT3 position;
		position.x = mPosition.x * (1/parentScale.x);
		position.y = mPosition.y * (1/parentScale.y);
		position.z = mPosition.z * (1/parentScale.z);
		XMVECTOR posVector = XMLoadFloat3(&position);
		XMVector3Transform(posVector, rotMat);
		XMStoreFloat3(&position, posVector);
		position.x -= parentPosition.x;
		position.y -= parentPosition.y;
		position.z -= parentPosition.z;
		mLocalPosition = position;
		//SetPosition(position);
	}


private:
	XMFLOAT3 mLocalPosition;
	XMFLOAT3 mLocalRotation;
	XMFLOAT3 mLocalScale;

	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
	XMMATRIX mTransform;

	TransformComponent* mParent;
	TransformComponent* mChild;
};

DECLARE_COMPONENT(TransformComponent);