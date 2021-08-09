#pragma once
#include<string>
#include <d3d11.h>

class Material
{
};

const std::string DEFAULT_TEXTURE_KEY = "White";

class SpcularMaterial : public Material 
{
public:
	SpcularMaterial():
		mTextureKey(DEFAULT_TEXTURE_KEY)
	{}
	SpcularMaterial(std::string texture) :
		mTextureKey(texture)
	{}
	~SpcularMaterial() = default;

	const std::string& GetTextureKey() const { return mTextureKey; }
	void SetTextureKey(std::string key) { mTextureKey = key; }
private:
	std::string mTextureKey;
};

class BumpSpacularMaterial : public Material
{
public:
	BumpSpacularMaterial() :
		mTextureKey(DEFAULT_TEXTURE_KEY),
		mNormalKey(DEFAULT_TEXTURE_KEY),
		mSpecularKey(DEFAULT_TEXTURE_KEY)
	{}
	BumpSpacularMaterial(std::string texture, std::string normal, std::string specular):
		mTextureKey(texture),
		mNormalKey(normal),
		mSpecularKey(specular)
	{}
	~BumpSpacularMaterial() = default;

	const std::string& GetTextureKey() const { return mTextureKey; }
	const std::string& GetNormalKey() const { return mNormalKey; }
	const std::string& GetSpecularKey() const { return mSpecularKey; }

	void SetTextureKey(std::string key) { mTextureKey = key; }
	void SetNormalKey(std::string key) { mNormalKey = key; }
	void SetSpecularKey(std::string key) { mSpecularKey = key; }
private:
	std::string mTextureKey;
	std::string mNormalKey;
	std::string mSpecularKey;
};
