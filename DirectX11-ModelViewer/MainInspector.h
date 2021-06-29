#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <unordered_map>
#include "GameObject.h"

class MainInspector
{
public:
	MainInspector();
	~MainInspector() = default;

	void Render(bool* pActiveModelBrowser,bool* pActiveAnimBrowser, std::unordered_map<std::string, GameObject>& meshMap, string& renderModelKey);

private:  
    unsigned int mModleCount;
	std::vector<string> mModelList;
	int mCurrentModelNum;
	string mCurretModelKey;
};

