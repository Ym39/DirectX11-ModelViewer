#pragma once
#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include "Utility.h"
#include "MeshClass.h"
using namespace std;

class AssetClass
{
public:
	static void Initialize();

private:
	static std::unordered_map<std::string, MeshClass*> mMeshMap;
	static std::filesystem::path mRootPath;
};

