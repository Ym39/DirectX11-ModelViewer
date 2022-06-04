#pragma once
#include <unordered_map>
#include <vector>
#include "Singleton.h"

using namespace std;

class GameObjectManager : public Singletone<GameObjectManager>
{
public:
	bool AddGameObject(string key, GameObjectClass* gameObject);
	vector<string>& GetAllGameObjectName();
	GameObjectClass* GetGameObjcet(string key);
	unordered_map<string, GameObjectClass*>& GetGetAllGameObject();

    void Shutdown();

private:
	unordered_map<string, GameObjectClass*> mGameObjects;
	vector<string> mGameObjectNames;
};

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

unordered_map<string, GameObjectClass*>& GameObjectManager::GetGetAllGameObject()
{
    return mGameObjects;
}

void GameObjectManager::Shutdown()
{
    for (auto& go : mGameObjects)
    {
        go.second->Destroy();
        delete go.second;
        go.second = nullptr;
    }
}
