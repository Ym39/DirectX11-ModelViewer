#pragma once
#include<array>
#include <d3d11.h>
#include <directxmath.h>
#include "Bounds.h"
using namespace DirectX;

constexpr static std::array<XMFLOAT3, 29> arrowPositions = {
	// Shaft
	XMFLOAT3(-0.2f, -0.2f, 0.f), XMFLOAT3(-0.2f, -0.2f, 0.75f), XMFLOAT3(-0.2f, 0.2f, 0.75f), XMFLOAT3(-0.2f, 0.2f, 0.f),
	XMFLOAT3(-0.2f, -0.2f, 0.75f), XMFLOAT3(-0.2f, 0.2f, 0.75f), XMFLOAT3(0.2f, 0.2f, 0.75f), XMFLOAT3(0.2f, -0.2f, 0.75f),
	XMFLOAT3(-0.2f, -0.2f, 0.f), XMFLOAT3(-0.2f, 0.2f, 0.f), XMFLOAT3(0.2f, 0.2f, 0.f), XMFLOAT3(0.2f, -0.2f, 0.f),
	XMFLOAT3(0.2f, -0.2f, 0.f), XMFLOAT3(0.2f, -0.2f, 0.75f), XMFLOAT3(0.2f, 0.2f, 0.75f), XMFLOAT3(0.2f, 0.2f, 0.f),
	XMFLOAT3(-0.2f, 0.2f, 0.f), XMFLOAT3(0.2f, 0.2f, 0.f), XMFLOAT3(0.2f, 0.2f, 0.75f), XMFLOAT3(-0.2f, 0.2f, 0.75f),
	XMFLOAT3(-0.2f, -0.2f, 0.f), XMFLOAT3(0.2f, -0.2f, 0.f), XMFLOAT3(0.2f, -0.2f, 0.75f), XMFLOAT3(-0.2f, -0.2f, 0.75f),

	// Head
	XMFLOAT3(-0.4f, -0.4f, 0.75f), XMFLOAT3(-0.4f, 0.4f, 0.75f), XMFLOAT3(0.4f, 0.4f, 0.75f), XMFLOAT3(0.4f, -0.4f, 0.75f),
	XMFLOAT3(0.f, 0.f, 1.f)
};

constexpr static std::array<size_t, 54> arrowIndice = {
	// Shaft
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	8, 9, 10, 8, 10, 11,
	12, 14, 13, 12, 15, 14,
	16, 18, 17, 16, 19, 18,
	20, 21, 22, 20, 22, 23,
	// Head
	24, 26, 25, 24, 27, 26,
	24, 28, 27, 24, 25, 28, 
	25, 26, 28, 26, 27, 28
};

enum class ArrowDirection
{
	Forward, Right, Up
};

class ArrowModel
{
public:
	ArrowModel();
	~ArrowModel();

	bool Initialize(ID3D11Device* device , ArrowDirection direction = ArrowDirection::Forward, XMFLOAT3 scale = XMFLOAT3(1.0f,1.0f,1.0f));
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount() const { return mIndexCount; }

	bool RayIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, XMFLOAT3 position , XMFLOAT3 arrowScale);

private:
	bool InitializeBuffers(ID3D11Device* device,XMFLOAT3 scale);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	void ComputeBounds();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount = 0;
	int mIndexCount = 0;

	Bounds mBounds;
	ArrowDirection mArrowDirection;
};

