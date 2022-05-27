#include "MaterialManager.h"

WideMaterial* MaterialManager::GetTexture(string key)
{
    if (mTextures.find(key) == mTextures.end())
        return nullptr;

    return mTextures[key];
}

bool MaterialManager::SetTexture(string key, WideMaterial* mat)
{
    if (mTextures.find(key) != mTextures.end())
        return false;

    mTextures[key] = mat;
    mTextureKeyNames.push_back(key);

    return true;
}

bool MaterialManager::AddNewTexture(string key)
{
    if (mTextures.find(key) != mTextures.end())
        return false;

    mTextures[key] = new WideMaterial;
    mTextureKeyNames.push_back(key);

    return true;
}

const std::vector<string>& MaterialManager::GetTextureKeyList()
{
    return mTextureKeyNames;
}

