#pragma once
#include <unordered_map>
#include <vector>
#include "Singleton.h"
#include "Material.h"
using namespace std;

class MaterialManager : public Singletone<MaterialManager>
{
public:
	WideMaterial* GetTexture(string key);

	/// <summary>
	/// 같은 key가 존재하면 false를 반환
	/// </summary>
	bool SetTexture(string key,WideMaterial* mat);
	bool AddNewTexture(string key);

    const std::vector<string>& GetTextureKeyList();

	MaterialManager() : Singletone()
	{
		mTextureKeyNames.reserve(50);
	}

private:
	std::unordered_map<string, WideMaterial*> mTextures;
	std::vector<string> mTextureKeyNames;
};

