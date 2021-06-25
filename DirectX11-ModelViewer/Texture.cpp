#include "Texture.h"

Texture::Texture():
mTexture(nullptr),
mTextureView(nullptr)
{

}

Texture::Texture(const Texture&)
{

}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	HRESULT result;
	DirectX::ScratchImage image = LoadTextureFromFile(filename);
	result = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &mTextureView);

	if (FAILED(result))
		return false;

	return true;
}

void Texture::Shutdown()
{
	if (mTextureView)
	{
		mTextureView->Release();
		mTextureView = nullptr;
	}

	if (mTexture)
	{
		mTexture->Release();
		mTexture = nullptr;
	}

	return;
}

DirectX::ScratchImage Texture::LoadTextureFromFile(LPCSTR filename)
{
	// Load the texture.
	string str(filename);
	wstring wsTmp(str.begin(), str.end());

	wstring ws = wsTmp;
	// Load the texture.
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(ws.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	HRESULT hr;
	ScratchImage image;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(ws.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}

	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = LoadFromTGAFile(ws.c_str(), nullptr, image);
	}

	else if (_wcsicmp(ext, L".wic") == 0)
	{
		hr = LoadFromWICFile(ws.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	else if (_wcsicmp(ext, L".png") == 0)
	{
		hr = LoadFromWICFile(ws.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	return image;
}
