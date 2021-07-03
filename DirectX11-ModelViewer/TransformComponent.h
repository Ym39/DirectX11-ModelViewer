#pragma once
#include "Component.h"
#include <directxmath.h>
using namespace DirectX;

class TransformComponent : BaseComponent
{
public:
	DEFINE_COMPONENT(TransformComponent, BaseComponent, true);

public:
	TransformComponent();
	~TransformComponent() = delete;

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	XMVECTOR GetPosition() const { return XMLoadFloat3(&mPosition); }
	XMFLOAT3 GetRotation() const { return mRotation; }
	XMFLOAT3 GetScale() const { return mScale; }

	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);

	void UpdateTransform();

	XMVECTOR Forward();
	XMVECTOR Right();
	XMVECTOR Up();

private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
	XMMATRIX mTransform;
};

