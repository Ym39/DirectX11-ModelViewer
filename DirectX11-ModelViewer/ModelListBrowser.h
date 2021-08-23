#pragma once
#include<vector>
#include<string>
#include<filesystem>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Camera.h"
#include "Light.h"

enum class RendererType
{
	ONLYSPECULAR,
	BUMP
};

class ModelListBrowser
{
public:
	ModelListBrowser();
	~ModelListBrowser() = default;

	void RenderGameObjectList(bool* addGameObject,std::string* outSelectGameObject,std::string* outSelectedModelKey,std::string* outSelectedTextureKey,std::string& selectedBumpTexture, std::string& selectedSpecularTexture, std::vector<std::string>& gameObejcts, RendererType& rendererType);

	void RenderModelList(bool& select, std::string& selectModelKey);

	void RenderAddGameObejctUI(bool* outAddGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey,std::string& selectedBumpTexture, std::string& selectedSpecularTexture, RendererType& rendererType);

	void RenderTextureList(bool& select, std::string& selectTextureKey);

	void RenderInspector();

	void RenderLoadFileWindow(bool* pActiveModelBrowser, bool* pActiveAnimBrowser);

	void RenderModelFileBrowser(bool* pLoadFbx, std::filesystem::path& filePath, bool* pActive);

	void RenderModelAnimationBrowser(bool* pLoadAnim, std::filesystem::path& filePath, bool* pActive);

	void RenderCameraWindow(Camera& camera);

	void RenderLightState(Light& light);
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
	int mCurrentSeletedBumpTextureNumber = 0;
	int mCurrentSeletedSpecularTextureNumber = 0;

	bool mSelectingModel = false;
	bool mSelectingTexture = false;

	std::string selectedModelKey = "";
	std::string selectedTextureKey = "";
};

