#pragma once
#include <directxmath.h>
#include "Material.h"
using namespace DirectX;

struct PerObejct
{
	XMMATRIX modelViewProjection;
	XMMATRIX modelView;
};

struct MaterialBuffer
{
	ShaderUploadMaterial mat;
};

struct LightBuffer
{
	XMFLOAT4 positionWS;
	XMFLOAT4 directionWS;
	XMFLOAT4 positionVS;
	XMFLOAT4 directionVS;
	XMFLOAT4 color;
	float spotlightAngle;
	float range;
	float intensity;
	UINT32 enabled;
	UINT32 selected;
	int type;
	XMFLOAT2 padding;
};