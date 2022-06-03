#include "GameObjectManager.h"

bool GameObjectManager::AddGameObject(string key, GameObjectClass* gameObject)
{
    auto it = mGameObjects.find(key);

    if (it != mGameObjects.end())
    {
        return false;
    }

    mGameObjects[key] = gameObject;
    mGameObjectNames.push_back(key);

    return true;
}

vector<string>& GameObjectManager::GetAllGameObjectName()
{
    return mGameObjectNames;
}

GameObjectClass* GameObjectManager::GetGameObjcet(string key)
{
    auto it = mGameObjects.find(key);

    if (it == mGameObjects.end())
    {
        return nullptr;
    }

    return mGameObjects[key];
}
