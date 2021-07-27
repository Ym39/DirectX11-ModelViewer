#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

class GameObjectClass;

class GameObjectBrowser
{
public:
	void Render(bool* addGameObject, std::vector<std::string>& gameObejcts);
private:
	std::vector<std::string> mGameObjectNames;
	bool mActive = true;
	int mCurrentItemNumber = 0;
};

