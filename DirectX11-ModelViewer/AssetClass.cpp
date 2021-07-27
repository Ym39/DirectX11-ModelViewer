#include "AssetClass.h"

std::filesystem::path AssetClass::mRootPath = filesystem::current_path();
std::unordered_map<std::string, MeshClass*> AssetClass::mMeshMap = {};
std::unordered_map<std::string, Texture*> AssetClass::mTextureMap = {};

AssetClass::~AssetClass()
{
	for (auto& mesh : mMeshMap)
	{
		mesh.second->Shutdown();
	}

	for (auto& texture : mTextureMap)
	{
		texture.second->Shutdown();
	}
}

void AssetClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	filesystem::path modelPath;
	filesystem::path currentPath = mRootPath;
	for (const auto entry : filesystem::directory_iterator(currentPath))
	{
		filesystem::path path = entry.path();
		if (entry.path().filename().string() == "Model" && entry.is_directory())
		{
			modelPath = entry.path();
		}
	}

	if (modelPath.empty() == false)
	{
		for (const auto entry : filesystem::directory_iterator(modelPath))
		{
			filesystem::path path = entry.path();
			if (entry.path().extension() == ".SM")
			{
				ifstream in;
				SkinnedMeshData mesh;
				in.open(entry.path().c_str(), ios_base::binary);
				boost::archive::binary_iarchive in_archive(in);
				in_archive >> mesh;
				in.close();

				MeshClass* loadMesh = new MeshClass;
				loadMesh->operator=(mesh);

				if (loadMesh->Initalize(device) == false)
					continue;

				mMeshMap[path.stem().string()] = loadMesh;
			}
		}
	}

	filesystem::path texturePath;
	for (const auto entry : filesystem::directory_iterator(currentPath))
	{
		filesystem::path path = entry.path();
		if (entry.path().filename().string() == "Texture" && entry.is_directory())
		{
			texturePath = entry.path();
		}
	}

	if (texturePath.empty() == false)
	{
		for (const auto entry : filesystem::directory_iterator(texturePath))
		{
			filesystem::path path = entry.path();
			if (entry.path().extension() == ".dds" || entry.path().extension() == ".png")
			{
				Texture* texture = new Texture();

				if (texture->Initialize(device, deviceContext, const_cast<char*>(path.string().c_str())) == false)
					continue;

				mTextureMap[path.stem().string()] = texture;
			}
		}
	}
}
