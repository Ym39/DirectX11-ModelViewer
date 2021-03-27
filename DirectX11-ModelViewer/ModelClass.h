#pragma once
#include <d3d11.h>
#include"Texture.h"
#include "Utility.h"
#include<fstream>

class ModelClass
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, const char* modelFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetPosition(float x, float y, float z);
	void GetPosition(float& x, float& y, float& z);

	void SetTexture(Texture* texture);

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool LoadModel(const char* modelFilename);
	void ReleaseModel();

private:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
	int mVertexCount = 0;
	int mIndexCount = 0;
	Texture* mTexture = nullptr;
	ModelType* mModel = nullptr;
	XMFLOAT3 mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

