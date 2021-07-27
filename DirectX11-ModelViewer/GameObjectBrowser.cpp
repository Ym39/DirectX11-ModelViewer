#include "GameObjectBrowser.h"
namespace ImGui
{
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
}

void GameObjectBrowser::Render(bool* addGameObject, std::vector<std::string>& gameObejcts)
{
	//mGameObjectNames.clear();

	//for (const auto& go : gameObjects)
	//{
	//	mGameObjectNames.push_back(go.first);
	//}

	ImGui::Begin("GameObject", &mActive, ImGuiWindowFlags_MenuBar);
	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &mCurrentItemNumber, ImGui::VectorGetter, static_cast<void*>(&gameObejcts), (int)gameObejcts.size(), 16);

	if (ImGui::Button("Add"))
	{
		*addGameObject = true;
	}
	else
	{
		*addGameObject = false;
	}

	ImGui::End();
}
