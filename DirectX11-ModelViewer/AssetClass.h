#pragma once
#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include "Texture.h"
#include "Utility.h"
#include "MeshClass.h"
using namespace std;

class AssetClass
{
public:
	~AssetClass();

	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void Update();

	static const auto& GetMeshs() { return mMeshMap; }
	static const auto& GetTextures() { return mTextureMap; }

	static const auto& GetModelPath() { return mModelPath; }

public:
	static std::unordered_map<std::string, MeshClass*> mMeshMap;
	static std::unordered_map<std::string, Texture*> mTextureMap;
private:
	static std::filesystem::path mRootPath;
	static std::filesystem::path mModelPath;
};

