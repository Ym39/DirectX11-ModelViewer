#include "MainInspector.h"

MainInspector::MainInspector():
mModleCount(0),
mCurrentModelNum(-1),
mCurretModelKey("")
{
    mModelList.reserve(20);
}

bool Getter(void* list, int count, const char** outText)
{
	std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
	if (count < 0 || count > vector.size())
	{
		return false;
	}
	*outText = vector[count].c_str();

	return true;
}


void MainInspector::Render(bool* pActiveModelBrowser, bool* pActiveAnimBrowser, std::unordered_map<std::string, GameObject>& meshMap, string& renderModelKey)
{
	if (mModleCount != meshMap.size())
	{
	    mModleCount = meshMap.size();
		mModelList.clear();
		for (std::pair<std::string,GameObject> mesh : meshMap)
		{
			mModelList.push_back(mesh.first);
		}
	}
 
	bool active = true;
	ImGui::Begin("Main",&active, ImGuiWindowFlags_MenuBar);
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

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &mCurrentModelNum, Getter, static_cast<void*>(&mModelList), (int)mModelList.size(), 8);
	if (select == true)
	{
		mCurretModelKey = mModelList[mCurrentModelNum];
	}
	

	/*std::string selectString = "False";
	if (ImGui::Button("Play"))
	{
	   meshMap[mCurretModelKey].PlayAnimation(true);
	   selectString = "true";
	}
	ImGui::Text("%s", selectString.c_str());*/

	ImGui::End();
}
