#include "ModelListBrowser.h"
#include "AssetClass.h"

bool VectorGetter(void* list, int count, const char** outText)
{
	std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
	if (count < 0 || count > vector.size())
	{
		return false;
	}
	*outText = vector[count].c_str();

	return true;
}

ModelListBrowser::ModelListBrowser():
	mSelectingModel(false)
{
}

void ModelListBrowser::RenderAddGameObejctUI(bool* outAddGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey,std::string& selectedBumpTexture, std::string& selectedSpecularTexture, RendererType& rendererType)
{
	ImGui::Begin("AddGameObject", &mActiveAddGameObject, ImGuiWindowFlags_None);

	static const char* rendererItems[] = { "OnlySpecular", "Bump" };
	static int currentRendererItem = 0;
	static int itemSize = IM_ARRAYSIZE(rendererItems);

	ImGui::Combo("RendererType", &currentRendererItem, rendererItems, itemSize);

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);

	ImGui::Text("Selected Model");

	mModelFileNames.clear();

	for (const auto& mesh : AssetClass::GetMeshs())
	{
		mModelFileNames.push_back(mesh.first);
	}

	bool selectModel = ImGui::Combo("ModelCombo", &mCurrentSeletedModelNumber, VectorGetter, static_cast<void*>(&mModelFileNames), mModelFileNames.size(),16);
    selectedModelKey = mModelFileNames[mCurrentSeletedModelNumber];

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);

	ImGui::Text("Selected Texture");

	mTextureNames.clear();

	for (const auto& texture : AssetClass::GetTextures())
	{
		mTextureNames.push_back(texture.first);
	}

	bool seletTexture = ImGui::Combo("TextureCombo", &mCurrentSeletedTextureNumber, VectorGetter, static_cast<void*>(&mTextureNames), mTextureNames.size(), 16);
	selectedTextureKey = mTextureNames[mCurrentSeletedTextureNumber];

	
	if (currentRendererItem == 1)
	{
		ImGui::NewLine();
		ImGui::PushItemWidth(-1);
		ImGui::Text("Selected Normal Map");
		ImGui::Combo("BumpTextureCombo", &mCurrentSeletedBumpTextureNumber, VectorGetter, static_cast<void*>(&mTextureNames), mTextureNames.size(), 16);
		selectedBumpTexture = mTextureNames[mCurrentSeletedBumpTextureNumber];
		ImGui::NewLine();
		ImGui::PushItemWidth(-1);
		ImGui::Text("Selected Specular Map");
		ImGui::Combo("SpecularTextureCombo", &mCurrentSeletedSpecularTextureNumber, VectorGetter, static_cast<void*>(&mTextureNames), mTextureNames.size(), 16);
		selectedSpecularTexture = mTextureNames[mCurrentSeletedSpecularTextureNumber];
	}

	ImGui::NewLine();
	if (ImGui::Button("Add"))
	{
		if (selectedModelKey != "" && selectedTextureKey != "")
		{
			*outSelectedModelKey = selectedModelKey;
			*outSelectedTextureKey = selectedTextureKey;
			*outAddGameObject = true;

			selectedModelKey = selectedTextureKey = "";
			
			switch (currentRendererItem)
			{
			case 0:
				rendererType = RendererType::ONLYSPECULAR;
				break;
			case 1:
				rendererType = RendererType::BUMP;
				break;
			}
		}
		mActiveAddGameObject = false;
	}

	ImGui::End();
}

void ModelListBrowser::RenderGameObjectList(bool* addGameObject, std::string* outSelectGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey, std::string& selectedBumpTexture, std::string& selectedSpecularTexture, std::vector<std::string>& gameObejcts, RendererType& rendererType)
{
	ImGui::Begin("GameObject", &mActiveGameObjectList, ImGuiWindowFlags_MenuBar);
	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &mCurrentGameObjectNumber, VectorGetter, static_cast<void*>(&gameObejcts), (int)gameObejcts.size(), 16);

	if (ImGui::Button("Add GameObject"))
	{
		mActiveAddGameObject = true;
	}

	ImGui::End();

	if (mActiveAddGameObject)
	{
		RenderAddGameObejctUI(addGameObject,outSelectedModelKey,outSelectedTextureKey,selectedBumpTexture,selectedSpecularTexture,rendererType);
	}

	if (gameObejcts.size() == 0)
		*outSelectGameObject = "";
	else
	{
		if(select == true)
		*outSelectGameObject = gameObejcts[mCurrentGameObjectNumber];
	}
}

void ModelListBrowser::RenderModelList(bool& select, std::string& selectModelKey)
{
	mModelFileNames.clear();

	for (const auto& mesh : AssetClass::GetMeshs())
	{
		mModelFileNames.push_back(mesh.first);
	}

	ImGui::Begin("Model", &mActiveRenderModelList, ImGuiWindowFlags_None);
	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	select = ImGui::ListBox("##listbox2", &mCurrentSeletedModelNumber, VectorGetter, static_cast<void*>(&mModelFileNames), (int)mModelFileNames.size(), 16);
	if (select)
	{
		selectModelKey = mModelFileNames[mCurrentSeletedModelNumber];
	}
	ImGui::End();
}

void ModelListBrowser::RenderTextureList(bool& select, std::string& selectTextureKey)
{
	mTextureNames.clear();

	for (const auto& texture : AssetClass::GetTextures())
	{
		mTextureNames.push_back(texture.first);
	}

	ImGui::Begin("Texture", &mActiveTextureList, ImGuiWindowFlags_MenuBar);
	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	select = ImGui::ListBox("##listbox2", &mCurrentSeletedTextureNumber, VectorGetter, static_cast<void*>(&mTextureNames), (int)mTextureNames.size(), 16);
	if (select)
	{
		selectTextureKey = mTextureNames[mCurrentSeletedTextureNumber];
	}
	ImGui::End();
}

void ModelListBrowser::RenderInspector()
{
	
}

void ModelListBrowser::RenderLoadFileWindow(bool* pActiveModelBrowser, bool* pActiveAnimBrowser)
{
	bool active = true;
	ImGui::Begin("Load Asset", &active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load FBX Model"))
			{
				*pActiveModelBrowser = true;
			}

			if (ImGui::MenuItem("Load Animation"))
			{
				*pActiveAnimBrowser = true;
			}


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void ModelListBrowser::RenderModelFileBrowser(bool* pLoadFbx, std::filesystem::path& filePath, bool* pActive)
{
	if (*pActive == false)
	{
		return;
	}

	static bool latest = false;
	static std::vector<std::string> currentDirectoryFileNames;
	static std::filesystem::path currentPath = std::filesystem::current_path();
	static std::vector<std::filesystem::path> popup;

	if (latest == false)
	{
		currentDirectoryFileNames.clear();
		for (const auto entry : std::filesystem::directory_iterator(currentPath))
		{
			if (entry.is_directory() || entry.path().extension() == ".fbx")
			{
				currentDirectoryFileNames.push_back(entry.path().filename().string());
			}
		}
		latest = true;
	}

	ImGui::Begin("Model File", pActive, ImGuiWindowFlags_MenuBar);

	ImGui::Text(currentPath.string().c_str());

	if (ImGui::Button("Root"))
	{
		currentPath = std::filesystem::current_path();
		popup.clear();
		latest = false;
	}

	for (int i = 0; i < popup.size(); ++i)
	{
		std::string directoryName = popup[i].filename().string();
		ImGui::SameLine();
		if (ImGui::Button(directoryName.c_str()))
		{
			currentPath = popup[i];
			popup.erase(popup.begin() + i + 1, popup.end());
			latest = false;
		}
	}

	static int currentItem = 0;
	static std::filesystem::path selectedPath;
	static string selectFilePath = "";

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &currentItem, VectorGetter, static_cast<void*>(&currentDirectoryFileNames), (int)currentDirectoryFileNames.size(), 16);
	if (select == true)
	{
		std::filesystem::path selectPath = currentPath / currentDirectoryFileNames[currentItem];
		if (std::filesystem::directory_entry(selectPath).is_directory())
		{
			currentPath = selectPath;
			popup.push_back(selectPath);
			latest = false;
		}
		else
		{
			selectedPath = selectPath;
			selectFilePath = selectPath.string();
		}
	}

	ImGui::Text(selectFilePath.c_str());

	if (ImGui::Button("Load"))
	{
		*pLoadFbx = true;
		filePath = selectedPath;
		*pActive = false;
	}

	ImGui::End();
}

void ModelListBrowser::RenderModelAnimationBrowser(bool* pLoadAnim, std::filesystem::path& filePath, bool* pActive)
{
	if (*pActive == false)
	{
		return;
	}

	static bool latest = false;
	static std::vector<std::string> currentDirectoryFileNames;
	static std::filesystem::path currentPath = std::filesystem::current_path();
	static std::vector<std::filesystem::path> popup;

	if (latest == false)
	{
		currentDirectoryFileNames.clear();
		for (const auto entry : std::filesystem::directory_iterator(currentPath))
		{
			if (entry.is_directory() || entry.path().extension() == ".fbx")
			{
				currentDirectoryFileNames.push_back(entry.path().filename().string());
			}
		}
		latest = true;
	}

	ImGui::Begin("AnimationFile", pActive, ImGuiWindowFlags_MenuBar);

	ImGui::Text(currentPath.string().c_str());

	if (ImGui::Button("Root"))
	{
		currentPath = std::filesystem::current_path();
		popup.clear();
		latest = false;
	}

	for (int i = 0; i < popup.size(); ++i)
	{
		std::string directoryName = popup[i].filename().string();
		ImGui::SameLine();
		if (ImGui::Button(directoryName.c_str()))
		{
			currentPath = popup[i];
			popup.erase(popup.begin() + i + 1, popup.end());
			latest = false;
		}
	}

	static int currentItem = 0;
	static std::filesystem::path selectedPath;
	static string selectFilePath = "";

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &currentItem, VectorGetter, static_cast<void*>(&currentDirectoryFileNames), (int)currentDirectoryFileNames.size(), 16);
	if (select == true)
	{
		std::filesystem::path selectPath = currentPath / currentDirectoryFileNames[currentItem];
		if (std::filesystem::directory_entry(selectPath).is_directory())
		{
			currentPath = selectPath;
			popup.push_back(selectPath);
			latest = false;
		}
		else
		{
			selectPath = selectedPath;
			selectFilePath = selectPath.string();
		}
	}

	ImGui::Text(selectFilePath.c_str());

	if (ImGui::Button("Load"))
	{
		*pLoadAnim = true;
		filePath = selectedPath;
		*pActive = false;
	}

	ImGui::End();
}



void ModelListBrowser::RenderCameraWindow(Camera& camera)
{
	XMFLOAT3 position = camera.GetPosition();
	XMFLOAT3 rotation = camera.GetRotation();
	float positionArray[3] = { position.x,position.y,position.z };
	float rotationArray[3] = { rotation.x,rotation.y,rotation.z };
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", positionArray, 1.0f, -FLT_MAX, FLT_MAX, "%.1f");
	ImGui::DragFloat3("Rotation", rotationArray, 1.0f, -FLT_MAX, FLT_MAX, "%.1f");
	ImGui::End();

	camera.SetPosition(positionArray[0], positionArray[1], positionArray[2]);
	camera.SetRotation(rotationArray[0], rotationArray[1], rotationArray[2]);
}

void ModelListBrowser::RenderLightState(Light& light)
{
	XMFLOAT4 ambientColor = light.GetAmbientColor();
	float ambientColorValue[4] = { ambientColor.x,ambientColor.y,ambientColor.z,ambientColor.w };
	XMFLOAT4 specularColor = light.GetSpecularColor();
	float specularColorValue[4] = { specularColor.x,specularColor.y,specularColor.z,specularColor.w };
	float specularPower = light.GetSpecularPower();
	XMFLOAT3 lightPosition = light.GetPosition();
	float positionValue[3] = { lightPosition.x,lightPosition.y,lightPosition.z };

	ImGui::Begin("Light");
	ImGui::ColorPicker4("AmbinetColor", ambientColorValue);
	ImGui::ColorPicker4("SpecularColor", specularColorValue);
	ImGui::DragFloat("SpecualrPower", &specularPower, 0.1f, 0.0f, 15.0f);
	ImGui::DragFloat3("LightPosition", positionValue, 1.0f, -1000.0f, 1000.0f);
	ImGui::End();

	light.SetAmbientColor(ambientColorValue[0], ambientColorValue[1], ambientColorValue[2], ambientColorValue[3]);
	light.SetSpecularColor(specularColorValue[0], specularColorValue[1], specularColorValue[2], specularColorValue[3]);
	light.SetSpecularPower(specularPower);
	light.SetPosition(positionValue[0], positionValue[1], positionValue[2]);
}


