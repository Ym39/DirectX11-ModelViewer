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

void ModelListBrowser::RenderAddGameObejctUI(bool* outAddGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey)
{

	ImGui::Begin("AddGameObject", &mActiveAddGameObject, ImGuiWindowFlags_None);


	ImGui::NewLine();
	ImGui::PushItemWidth(20);
	ImGui::Text("Selected Model : %s", selectedModelKey.c_str());
	ImGui::SameLine();
	if (ImGui::Button("Search Model"))
	{
		mActiveRenderModelList = true;
	}

	ImGui::NewLine();
	ImGui::PushItemWidth(20);
	ImGui::Text("Selected Texture : %s", selectedTextureKey.c_str());
	ImGui::SameLine();
	if (ImGui::Button("Search Texture"))
	{
		mActiveTextureList = true;
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
		}
	}

	ImGui::End();

	bool selectedModel = false;
	if (mActiveRenderModelList == true)
	{
		RenderModelList(selectedModel, selectedModelKey);
	}

	bool selectedTexture = false;
	if (mActiveTextureList == true)
	{
		RenderTextureList(selectedTexture, selectedTextureKey);
	}
}

void ModelListBrowser::RenderGameObjectList(bool* addGameObject, std::string* outSelectedModelKey, std::string* outSelectedTextureKey, std::vector<std::string>& gameObejcts)
{
	ImGui::Begin("GameObject", &mActiveGameObjectList, ImGuiWindowFlags_MenuBar);
	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &mCurrentSeletedTextureNumber, VectorGetter, static_cast<void*>(&gameObejcts), (int)gameObejcts.size(), 16);

	if (ImGui::Button("Add GameObject"))
	{
		mActiveAddGameObject = true;
	}

	ImGui::End();

	if (mActiveAddGameObject)
	{
		RenderAddGameObejctUI(addGameObject,outSelectedModelKey,outSelectedTextureKey);
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


