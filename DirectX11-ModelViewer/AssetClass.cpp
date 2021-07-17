#include "AssetClass.h"

std::filesystem::path AssetClass::mRootPath = filesystem::current_path();
std::unordered_map<std::string, MeshClass*> AssetClass::mMeshMap = {};

void AssetClass::Initialize()
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

	if (modelPath.empty())
		return;

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

			mMeshMap[path.stem().string()] = loadMesh;
		}
	}
}
