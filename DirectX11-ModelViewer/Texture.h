#pragma once
#include<d3d11.h>
#include<stdio.h>
#include"DirectXTex.h"
#include<string>
using namespace DirectX;
using namespace std;

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void Shutdown();

	inline ID3D11ShaderResourceView* GetTexture() const {return mTextureView;}
	inline ID3D11ShaderResourceView* const* GetTexturePP() const
	{
		return &mTextureView; 
	}
private:
	ScratchImage LoadTextureFromFile(LPCSTR filename);
private:
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTextureView;
};

