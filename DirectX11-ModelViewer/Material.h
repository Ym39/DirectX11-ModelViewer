#pragma once
#include<string>
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

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

struct ShaderUploadMaterial
{
	XMFLOAT4 globalAmbient;
	XMFLOAT4 ambientColor;
	XMFLOAT4 emissiveColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT4 specularColor;
	XMFLOAT4 reflectance;
	float opacity;
	float specularPower;
	float indexOfRefraction;
	UINT32 hasAmbientTexture;
	UINT32 hasEmissiveTexture;
	UINT32 hasDiffuseTexture;
	UINT32 hasSpecularTexture;
	UINT32 hasSpecularPowerTexture;
	UINT32 hasNormalTexture;
	UINT32 hasBumpTexture;
	UINT32 hasOpacityTexture;
	float bumpIntensity;
	float specularScale;
	float alphaThreshold;
	XMFLOAT2 padding;
};

class WideMaterial : public Material
{
public:
	WideMaterial() :
		mAmbientKey(DEFAULT_TEXTURE_KEY),
		mEmissiveKey(DEFAULT_TEXTURE_KEY),
		mDiffuseKey(DEFAULT_TEXTURE_KEY),
		mSpecularKey(DEFAULT_TEXTURE_KEY),
		mSpecularPower(DEFAULT_TEXTURE_KEY),
		mNormalKey(DEFAULT_TEXTURE_KEY),
		mBumpKey(DEFAULT_TEXTURE_KEY),
		mOpacityKey(DEFAULT_TEXTURE_KEY)
	{
		shaderUploadMaterial.globalAmbient = XMFLOAT4();
		shaderUploadMaterial.ambientColor = XMFLOAT4();
		shaderUploadMaterial.emissiveColor = XMFLOAT4();
		shaderUploadMaterial.diffuseColor = XMFLOAT4();
		shaderUploadMaterial.specularColor = XMFLOAT4();
		shaderUploadMaterial.reflectance = XMFLOAT4();
		shaderUploadMaterial.opacity = 1.0f;
		shaderUploadMaterial.specularPower = 1.0f;
		shaderUploadMaterial.indexOfRefraction = 1.0f;

		shaderUploadMaterial.hasAmbientTexture = 0;
		shaderUploadMaterial.hasEmissiveTexture = 0;
		shaderUploadMaterial.hasDiffuseTexture = 0;
		shaderUploadMaterial.hasSpecularTexture = 0;
		shaderUploadMaterial.hasSpecularPowerTexture = 0;
		shaderUploadMaterial.hasNormalTexture = 0;
		shaderUploadMaterial.hasBumpTexture = 0;
		shaderUploadMaterial.hasOpacityTexture = 0;

		shaderUploadMaterial.bumpIntensity = 0.0f;
		shaderUploadMaterial.specularScale = 0.0f;
		shaderUploadMaterial.alphaThreshold = 0.0f;
	}
	~WideMaterial() = default;

	void SetAmbientKey(std::string key) { mAmbientKey = key; shaderUploadMaterial.hasAmbientTexture = 1; }
	void SetEmissiveKey(std::string key) { mEmissiveKey = key; shaderUploadMaterial.hasEmissiveTexture = 1; }
	void SetDiffuseKey(std::string key) { mDiffuseKey = key; shaderUploadMaterial.hasDiffuseTexture = 1; }
	void SetSpecularKey(std::string key) { mSpecularKey = key; shaderUploadMaterial.hasSpecularTexture = 1; }
	void SetSpecularPowerKey(std::string key) { mSpecularPower = key; shaderUploadMaterial.hasSpecularPowerTexture = 1; }
	void SetNormalKey(std::string key) { mNormalKey = key; shaderUploadMaterial.hasNormalTexture = 1; }
	void SetBumpKey(std::string key) { mBumpKey = key; shaderUploadMaterial.hasBumpTexture = 1; }
	void SetOpacityKey(std::string key) { mOpacityKey = key; shaderUploadMaterial.hasOpacityTexture = 1; }

	std::string GetAmbientKey() { return mAmbientKey; }
	std::string GetEmissiveKey() { return mEmissiveKey; }
	std::string GetDiffuseKey() { return mDiffuseKey; }
	std::string GetSpecularKey() { return mSpecularKey; }
	std::string GetSpecularPowerKey() { return mSpecularPower; }
	std::string GetNormalKey() { return mNormalKey; }
	std::string GetBumpKey() { return mBumpKey; }
	std::string GetOpacityKey() { return mOpacityKey; }

	void SetGlobalAmbient(float x, float y, float z, float w) 
	{
		shaderUploadMaterial.globalAmbient.x = x;
		shaderUploadMaterial.globalAmbient.y = y;
		shaderUploadMaterial.globalAmbient.z = z;
		shaderUploadMaterial.globalAmbient.w = w;
	}

	void SetAmbientColor(float x, float y, float z, float w)
	{
		shaderUploadMaterial.ambientColor.x = x;
		shaderUploadMaterial.ambientColor.y = y;
		shaderUploadMaterial.ambientColor.z = z;
		shaderUploadMaterial.ambientColor.w = w;
	}

	void SetEmissiveColor(float x, float y, float z, float w)
	{
		shaderUploadMaterial.emissiveColor.x = x;
		shaderUploadMaterial.emissiveColor.y = y;
		shaderUploadMaterial.emissiveColor.z = z;
		shaderUploadMaterial.emissiveColor.w = w;
	}

	void SetDiffuseColor(float x, float y, float z, float w)
	{
		shaderUploadMaterial.diffuseColor.x = x;
		shaderUploadMaterial.diffuseColor.y = y;
		shaderUploadMaterial.diffuseColor.z = z;
		shaderUploadMaterial.diffuseColor.w = w;
	}

	void SetSpecularColor(float x, float y, float z, float w)
	{
		shaderUploadMaterial.specularColor.x = x;
		shaderUploadMaterial.specularColor.y = y;
		shaderUploadMaterial.specularColor.z = z;
		shaderUploadMaterial.specularColor.w = w;
	}

	void Setreflectance(float x, float y, float z, float w)
	{
		shaderUploadMaterial.reflectance.x = x;
		shaderUploadMaterial.reflectance.y = y;
		shaderUploadMaterial.reflectance.z = z;
		shaderUploadMaterial.reflectance.w = w;
	}
public:
	ShaderUploadMaterial shaderUploadMaterial;

private:
	std::string mAmbientKey;
	std::string mEmissiveKey;
	std::string mDiffuseKey;
	std::string mSpecularKey;
	std::string mSpecularPower;
	std::string mNormalKey;
	std::string mBumpKey;
	std::string mOpacityKey;
};
