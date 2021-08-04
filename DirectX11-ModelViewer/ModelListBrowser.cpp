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
		*outSelectGameObject = gameObejcts[mCurrentGameObjectNumber];
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


