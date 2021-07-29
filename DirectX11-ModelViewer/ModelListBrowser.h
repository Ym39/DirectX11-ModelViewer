#pragma once
#include<vector>
#include<string>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

class ModelListBrowser
{
public:
	ModelListBrowser();
	~ModelListBrowser() = default;

	void RenderGameObjectList(bool* addGameObject,std::string* outSelectGameObject,std::string* outSelectedModelKey,std::string* outSelectedTextureKey, std::vector<std::string>& gameObejcts);

	void RenderModelList(bool& select, std::string& selectModelKey);

	void RenderAddGameObejctUI(bool* outAddGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey);

	void RenderTextureList(bool& select, std::string& selectTextureKey);

	void RenderInspector();

private:
	std::vector<std::string> mGameObjectNames;
	std::vector<std::string> mModelFileNames;
	std::vector<std::string> mTextureNames;
	bool mActiveGameObjectList = true;
	bool mActiveRenderModelList = false;
	bool mActiveTextureList = false;
	bool mActiveAddGameObject = false;
	int mCurrentGameObjectNumber = 0;
	int mCurrentSeletedModelNumber = 0;
	int mCurrentSeletedTextureNumber = 0;

	bool mSelectingModel = false;
	bool mSelectingTexture = false;

	std::string selectedModelKey = "";
	std::string selectedTextureKey = "";
};

