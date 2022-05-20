#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#define NUM_OF_LIGHTS 8

enum class LightType
{
	Point = 0,
	Spot = 1,
	Directional = 2
};

class LightSturct
{
public:
	XMFLOAT4 position;
	XMFLOAT4 direction;
	XMFLOAT4 color;
	float intensity;
	bool enabled;
	bool selected;
	LightType lightType;

public:
	LightSturct()
	{
		position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		direction = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		intensity = 1.0f;
		enabled = false;
		selected = false;
		lightType = LightType::Point;
	}
};

class LightManager
{
public:
	static LightManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new LightManager;
		}

		return *mInstance;
	}

	LightSturct& GetDirectionalLight();

	LightSturct* GetLight(int index);

	LightSturct* GetAllLight() { return mLight; }

private:
	// 0번 인덱스 : 디렉셔널 라이트
	// 1~7번 인덱스 : 포인트, 스포트 라이트
	LightSturct mLight[NUM_OF_LIGHTS];

private:
	static LightManager* mInstance;
};

