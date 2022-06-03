#pragma once
#include <unordered_map>
#include <vector>
#include "Singleton.h"
#include "Component.h"
using namespace std;

class GameObjectManager : public Singletone<GameObjectManager>
{
public:
	bool AddGameObject(string key, GameObjectClass* gameObject);
	vector<string>& GetAllGameObjectName();
	GameObjectClass* GetGameObjcet(string key);
	unordered_map<string, GameObjectClass*>& GetGetAllGameObject;

private:
	unordered_map<string, GameObjectClass*> mGameObjects;
	vector<string> mGameObjectNames;
};

