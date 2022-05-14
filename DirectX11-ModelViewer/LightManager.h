#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

enum class LightType
{
	Point = 0,
	Spot = 1,
	Directional = 2
};

struct Light
{
	XMFLOAT4 position;
	XMFLOAT4 direction;
	XMFLOAT4 color;
	float intensity;
	bool enabled;
	bool selected;
	LightType lightType;
};

class LightManager
{
public:
	static const LightManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new LightManager;
		}

		return *mInstance;
	}

	Light& GetDirectionalLight();

	Light& GetLight(int index);

private:
	// 0�� �ε��� : �𷺼ų� ����Ʈ
	// 1~7�� �ε��� : ����Ʈ, ����Ʈ ����Ʈ
	Light mLight[8];

private:
	static LightManager* mInstance;
};

