#pragma once
#include <unordered_map>
#include <vector>
#include "Singleton.h"
#include "Material.h"
using namespace std;

#define DEFAULT_MATRIAL_KEY "Default"

class MaterialManager : public Singletone<MaterialManager>
{
public:
	WideMaterial* GetMaterial(string key);

	/// <summary>
	/// ���� key�� �����ϸ� false�� ��ȯ
	/// </summary>
	bool SetMaterial(string key, WideMaterial* mat);
	bool AddNewMaterial(string key);

    std::vector<string>& GetMaterialKeyList();

	MaterialManager() : Singletone()
	{
		mMaterialKeyNames.reserve(50);
	}

	void Initialize()
	{
		SetMaterial(DEFAULT_MATRIAL_KEY, new WideMaterial);
	}

private:
	std::unordered_map<string, WideMaterial*> mMaterials;
	std::vector<string> mMaterialKeyNames;
};

