#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

enum class Axis
{
	Xaxis,Yaxis,Zaxis
};

class Transform
{
public:
   Transform();
   ~Transform() = default;

   XMFLOAT3 GetPosition() const;
   XMFLOAT3 GetRotation() const;
   XMFLOAT3 GetScale() const;
   XMMATRIX GetTransform() const;

   void Rotate(float delta, Axis axis);
   void Translate(XMFLOAT3 delta);

   void SetPosition(XMFLOAT3 position);
   void SetRotation(XMFLOAT3 rotation);
   void SetScale(XMFLOAT3 scale);

   XMFLOAT3 Forward();
   XMFLOAT3 Right();
   XMFLOAT3 Up();

private:
   XMFLOAT3 mPosition;
   XMFLOAT3 mRotation;
   XMFLOAT3 mScale;
};

