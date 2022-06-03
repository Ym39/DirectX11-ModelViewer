#include "MaterialManager.h"

WideMaterial* MaterialManager::GetMaterial(string key)
{
    if (mMaterials.find(key) == mMaterials.end())
        return nullptr;

    return mMaterials[key];
}

bool MaterialManager::SetMaterial(string key, WideMaterial* mat)
{
    if (mMaterials.find(key) != mMaterials.end())
        return false;

    mMaterials[key] = mat;
    mMaterialKeyNames.push_back(key);

    return true;
}

bool MaterialManager::AddNewMaterial(string key)
{
    if (mMaterials.find(key) != mMaterials.end())
        return false;

    mMaterials[key] = new WideMaterial;
    mMaterialKeyNames.push_back(key);

    return true;
}

std::vector<string>& MaterialManager::GetMaterialKeyList()
{
    return mMaterialKeyNames;
}

