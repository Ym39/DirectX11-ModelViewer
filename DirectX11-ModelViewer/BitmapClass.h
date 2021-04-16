#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include<fstream>
using namespace DirectX;
using namespace std;

#include"Texture.h"

class BitmapClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

	int GetIndexCount() const { return mIndexCount; }
	ID3D11ShaderResourceView* GetTexture() const { return mTexture->GetTexture(); }

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	XMFLOAT3 PointToNdc(int x, int y);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void ReleaseTexture();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
	Texture* mTexture;
	int mScreenWidth, mScreenHeight;
	int mBitmapWidth, mBitmapHeight;
	int mPreviousPosX, mPreviousPosY;

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass() = default;
};

